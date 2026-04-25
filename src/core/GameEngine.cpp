#include "GameEngine.hpp"
#include "AuctionController.hpp"
#include "BankruptcyController.hpp"
#include "EffectController.hpp"
#include "DisplayView.hpp"
#include "GameException.hpp"
#include "InputHandler.hpp"
#include "GameLogger.hpp"

using namespace std;

void GameEngine::initGame(GameContext& gameContext, TurnController& turnController, ConfigReader& configReader, EconomyController& economyController) {
    configReader.loadAllConfigs(&gameContext, gameContext.getBoard());

    CardDeck<ActionCard>& chanceDeck = gameContext.getChanceDeck();
    chanceDeck.add(new MoveToStationCard());
    chanceDeck.add(new MoveBackwardCard());
    chanceDeck.add(new MoveToJailCard());
    chanceDeck.initShuffle();

    CardDeck<ActionCard>& comChestDeck = gameContext.getCommunityChestDeck();
    comChestDeck.add(new BirthdayCard());
    comChestDeck.add(new DoctorFeeCard());
    comChestDeck.add(new NyalegCard());
    comChestDeck.initShuffle();

    CardDeck<SkillCard>& skillDeck = gameContext.getSkillDeck();
    for (int i = 0; i < 4; ++i) {
        skillDeck.add(new MoveCard());
    }

    for (int i = 0; i < 3; ++i) {
        skillDeck.add(new DiscountCard()); 
    }

    for (int i = 0; i < 2; ++i) {
        skillDeck.add(new ShieldCard());
    }

    for (int i = 0; i < 2; ++i) {
        skillDeck.add(new TeleportCard());
    }

    for (int i = 0; i < 2; ++i) {
        skillDeck.add(new LassoCard());
    }

    for (int i = 0; i < 2; ++i) {
        skillDeck.add(new DemolitionCard());
    }

    for (int i = 0; i < 2; ++i) {
        skillDeck.add(new JailFreeCard());
    }

    skillDeck.initShuffle();
}

void GameEngine::run() {
    string filePath = "Ayam";
    GameContext gameContext;
    ConfigReader configReader(filePath);
    SaveLoader saveLoader;
    TurnController turnController;
    EconomyController economyController;
    EffectController effectController;
    AuctionController auctionController;
    BankruptcyController bankruptcyController;
    InputHandler inputHandler(cin);
    GameLogger logger;
    DisplayView displayView;
    Dice dice;

    initGame(gameContext, turnController, configReader, economyController);

    bool gameReady = false;

    while (!gameReady) {
        displayView.renderStart();

        inputHandler.getIntInput();
        int menuChoice = inputHandler.getIntValue1();

        if (menuChoice == 1) {
            displayView.renderInfo("\n--- NEW GAME ---\n");
            int numPlayers = 0;
            while (numPlayers < 2 || numPlayers > 4) { 
                displayView.renderInfo("Input number of players (2-4): ");
                inputHandler.getIntInput();
                numPlayers = inputHandler.getIntValue1();
                if (numPlayers < 2 || numPlayers > 4) {
                    displayView.renderInfo("Number of players must be 2, 3, or 4!");
                }
            }

            for (int i = 0; i < numPlayers; ++i) {
                displayView.renderInfo("Input name of Player " + to_string(i + 1) + ": ");
                inputHandler.getStringInput();
                string pName = inputHandler.getLastStringInput();
                
                Player newPlayer(pName);
                newPlayer.setBalance(gameContext.getStartingMoney()); 
                gameContext.getPlayers().push_back(newPlayer);
            }
            
            srand(static_cast<unsigned>(time(0)));
            int startingIndex = rand() % numPlayers;

            gameContext.setCurrentPlayerIndex(startingIndex);
            
            displayView.renderInfo("\n[RANDOMIZING STARTING PLAYER]...");
            displayView.renderInfo("\n>> Game will start with: " + gameContext.getPlayers()[startingIndex].getName() + "!\n");
            gameReady = true;

        } else if (menuChoice == 2) {
            displayView.renderInfo("\n--- LOAD GAME ---\n");
            displayView.renderInfo("Input save file name: ");
            inputHandler.getStringInput();
            string saveFile = inputHandler.getLastStringInput();
            
            saveLoader.loadGame(saveFile, gameContext, logger); 
            
            if (!gameContext.getPlayers().empty()) {
                displayView.renderInfo("\n[SUCCESS] Load successful!\n");
                gameReady = true;
            } else {
                displayView.renderInfo("\n[ERROR] Failed to load save file or data is empty. Please try again.\n");
            }
        } else {
            displayView.renderInfo("\n[ERROR] Invalid choice!\n");
            inputHandler.clearInputBuffer();
        }
    }

    if (gameContext.getPlayers().empty()) {
        displayView.renderInfo("Player list is empty. Add players before running the game loop.");
        return; 
    }

    Player* previousPlayer = nullptr;
    bool hasUsedSkillThisTurn = false;

    while (!gameContext.isGameOver()) {
        
        if (gameContext.getCurrentPlayerIndex() == 0) {
            turnController.distributeSkillCards(gameContext, inputHandler, displayView);
        }

        Player* currentPlayer = &gameContext.getCurrentPlayer();
        
        if (currentPlayer->getStatus() == PlayerStatus::BANKRUPT) {
            gameContext.nextPlayer();
            continue;
        }

        if (currentPlayer != previousPlayer) {
            hasUsedSkillThisTurn = false;
            previousPlayer = currentPlayer;
        }

        displayView.renderInfo("============================================");
        displayView.renderInfo("Turn: " + currentPlayer->getName() + " (Round " + to_string(gameContext.getCurrentTurn()) + ")");
        
        bool turnEnded = false;
        bool isDoubleRoll = false;
        bool hasRolledDice = false;

        if (currentPlayer->getStatus() == PlayerStatus::JAILED) {
            int currentAttempt = currentPlayer->getJailTurns() + 1;
            currentPlayer->setJailTurns(currentAttempt);

            displayView.renderInfo("[IN JAIL] Attempt #" + to_string(currentAttempt));

            if (currentAttempt >= 4) {
                displayView.renderInfo("You have spent 3 turns in jail. You must pay a fine of M" + to_string(gameContext.getJailFine()) + "!");
                try {
                    *currentPlayer -= gameContext.getJailFine();
                    currentPlayer->setStatus(PlayerStatus::ACTIVE);
                    currentPlayer->setJailTurns(0);
                    displayView.renderInfo("Fine paid. You are free now!");
                } catch (const InsufficientFundsException& ex) {
                    bankruptcyController.liquidateAssets(gameContext, *currentPlayer, nullptr, ex.getRequired(), displayView, economyController, inputHandler, nullptr);
                    turnEnded = true;
                }
            } else {
                bool validJailAction = false;

                while (!validJailAction) {
                    displayView.renderInfo("Jail escape options:");
                    displayView.renderInfo("1. Pay fine (M" + to_string(gameContext.getJailFine()) + ")");
                    displayView.renderInfo("2. Use 'Get Out of Jail' card");
                    displayView.renderInfo("3. Roll dice (must be doubles)");
                    displayView.renderPrompt("Choice (1/2/3): ");

                    inputHandler.getIntInput();
                    int choice = inputHandler.getIntValue1(); 

                    if (choice == 1) {
                        try {
                            *currentPlayer -= gameContext.getJailFine();
                            currentPlayer->setStatus(PlayerStatus::ACTIVE);
                            currentPlayer->setJailTurns(0);
                            displayView.renderInfo("Fine paid. You are now free!");
                            validJailAction = true;
                        } catch (const InsufficientFundsException& ex) {
                            displayView.renderWarning("Insufficient funds!");
                            bankruptcyController.liquidateAssets(gameContext, *currentPlayer, nullptr, ex.getRequired(), displayView, economyController, inputHandler, nullptr);
                            turnEnded = true;
                            validJailAction = true;
                        }
                    } else if (choice == 2) {
                        int jailFreeIdx = currentPlayer->searchJailFreeCard();
                        
                        if (jailFreeIdx != -1) {
                            displayView.renderInfo("Using 'Get Out of Jail' card...");
                            
                            SkillCard* usedCard = currentPlayer->useSkillCard(jailFreeIdx);
                            
                            if (usedCard != nullptr) {
                                gameContext.getSkillDeck().discard(usedCard);
                            }

                            effectController.execute(*usedCard, *currentPlayer, gameContext, inputHandler, displayView);
                            validJailAction = true;
                        } else {
                            displayView.renderWarning("FAILED: You do not have a 'Get Out of Jail' card! Choose another option.");
                        }
                    } else if (choice == 3) {
                        displayView.renderInfo("Attempting to roll doubles...");
                        dice.roll();
                        displayView.renderInfo("Result: " + to_string(dice.getDice1()) + " + " + to_string(dice.getDice2()) + " = " + to_string(dice.getTotal()));
                        if (dice.isDouble()) {
                            displayView.renderInfo("DOUBLE! You are free and move immediately."); 
                            currentPlayer->setStatus(PlayerStatus::ACTIVE);
                            currentPlayer->setJailTurns(0);
                            try {
                                turnController.handleDiceRollMovement(&gameContext, economyController, effectController, auctionController, bankruptcyController, dice, saveLoader, inputHandler, logger, displayView);
                            } catch (const AuctionTriggerException&) {
                                auctionController.startAuctionSkipBuy(gameContext, displayView, inputHandler);
                                turnEnded = true;
                            } catch (const BankruptcyException& ex) {
                                bankruptcyController.liquidateAssets(gameContext, *currentPlayer, nullptr, ex.getRequired(), displayView, economyController, inputHandler, ex.getBankruptTile());
                                turnEnded = true;
                            }
                            hasRolledDice = true;
                            isDoubleRoll = true;
                        } else {
                            displayView.renderInfo("Failed to roll doubles. You remain in jail.");
                            turnEnded = true;
                        }
                        validJailAction = true;
                    } else {
                        displayView.renderWarning("Invalid choice!");
                        inputHandler.clearInputBuffer();
                    }
                }
            }
        }

        while (!turnEnded && currentPlayer->getStatus() == PlayerStatus::ACTIVE) {
            displayView.renderPrompt("\n[" + currentPlayer->getName() + "] Enter Command: ");
            CommandType cmd = inputHandler.getCommand();

            switch (cmd) {
                case CommandType::LEMPAR_DADU:
                    if (hasRolledDice) {
                        displayView.renderWarning("You have already rolled the dice this turn! Complete another action or end your turn.");
                        break;
                    }
                    dice.roll();
                    if (dice.isDouble()) {
                        isDoubleRoll = true;
                        displayView.renderInfo("\n " + currentPlayer->getName() + " rolled DOUBLES! Gets an extra turn! \n");
                    }
                    try {
                        displayView.renderDiceRoll(gameContext, dice);
                        turnController.handleDiceRollMovement(&gameContext, economyController, effectController, auctionController, bankruptcyController, dice, saveLoader, inputHandler, logger, displayView);
                    } catch (const AuctionTriggerException&) {
                        auctionController.startAuctionSkipBuy(gameContext, displayView, inputHandler);
                    } catch (const BankruptcyException& ex) {
                        bankruptcyController.liquidateAssets(gameContext, *currentPlayer, nullptr, ex.getRequired(), displayView, economyController, inputHandler, ex.getBankruptTile());
                    }
                    hasRolledDice = true;
                    break;

                case CommandType::ATUR_DADU: {
                    if (hasRolledDice) {
                        displayView.renderWarning("You have already rolled the dice this turn!");
                        break;
                    }
                    inputHandler.getIntTwoInput();
                    int x = inputHandler.getIntValue1();
                    int y = inputHandler.getIntValue2();
                    dice.setRoll(x, y);
                    if (dice.isDouble()) {
                        isDoubleRoll = true;
                        displayView.renderInfo("\n" + currentPlayer->getName() + " set DOUBLES! Gets an extra turn! \n");
                    }
                    try {
                        displayView.renderDiceRoll(gameContext, dice);
                        turnController.handleDiceRollMovement(&gameContext, economyController, effectController, auctionController, bankruptcyController, dice, saveLoader, inputHandler, logger, displayView);
                    } catch (const AuctionTriggerException&) {
                        auctionController.startAuctionSkipBuy(gameContext, displayView, inputHandler);
                    } catch (const BankruptcyException& ex) {
                        bankruptcyController.liquidateAssets(gameContext, *currentPlayer, ex.getCreditor(), ex.getRequired(), displayView, economyController, inputHandler, ex.getBankruptTile());
                    }
                    hasRolledDice = true;
                    break;
                }

                case CommandType::CETAK_PAPAN:
                    displayView.renderBoard(gameContext);
                    break;

                case CommandType::CETAK_AKTA: {
                    inputHandler.getStringInput();
                    displayView.renderAkta(gameContext, inputHandler.getLastStringInput());
                    break;
                }

                case CommandType::CETAK_PROPERTI:
                    displayView.renderProperty(gameContext);
                    break;

                case CommandType::GADAI:
                case CommandType::TEBUS:
                case CommandType::BANGUN:
                    break;

                case CommandType::GUNAKAN_KEMAMPUAN: {
                    if (hasRolledDice) {
                        displayView.renderWarning("Skill cards can ONLY be used BEFORE rolling the dice!");
                        break;
                    }

                    if (hasUsedSkillThisTurn) {
                        displayView.renderWarning("You can use a MAXIMUM of 1 Skill Card per turn!");
                        break;
                    }

                    if (!currentPlayer->hasAnySkillCard()) {
                        displayView.renderInfo("[ERROR] You do not have any Skill Cards!");
                        break;
                    }

                    displayView.renderInfo("Your Skill Cards:");
                    int displayIdx = 1;
                    for (SkillCard* card : currentPlayer->getSkillCard()) {
                        displayView.renderInfo("[" + to_string(displayIdx) + "] " + card->getName() + " - " + card->getDescription());
                        displayIdx++;
                    }
                    displayView.renderInfo("[0] Cancel card usage.");
                    displayView.renderPrompt("Choose a card to use (0-" + to_string(currentPlayer->getSkillCardCount()) + "): ");

                    int choice = -1;
                    while (true) {
                        inputHandler.getIntInput();
                        choice = inputHandler.getIntValue1();

                        if (choice >= 0 && choice <= currentPlayer->getSkillCardCount()) {
                            break;
                        }
                        displayView.renderPrompt("Invalid choice! Enter a number (0-" + to_string(currentPlayer->getSkillCardCount()) + "): ");
                    }

                    if (choice == 0) {
                        displayView.renderInfo("Skill card usage canceled.");
                        break;
                    }

                    int vectorIndex = choice - 1;
                    SkillCard* cardToUse = currentPlayer->dropSkillCard(vectorIndex);

                    displayView.renderInfo("\n>> Activating card: [" + cardToUse->getName() + "]...");

                    vector<int> preSkillPositions;
                    for (Player& p : gameContext.getPlayers()) {
                        preSkillPositions.push_back(p.getPosition());
                    }

                    effectController.execute(*cardToUse, *currentPlayer, gameContext, inputHandler, displayView);
                    gameContext.getSkillDeck().discard(cardToUse);
                    hasUsedSkillThisTurn = true;

                    for (int i = 0; i < gameContext.getPlayers().size(); i++) {
                        Player* targetP = &gameContext.getPlayers()[i];
                        if (targetP->getStatus() == PlayerStatus::BANKRUPT) continue;

                        if (targetP->getPosition() != preSkillPositions[i]) {
                            if (targetP == currentPlayer) {
                                displayView.renderInfo("\n[CARD EFFECT] You have moved to a new tile!");
                            } else {
                                displayView.renderInfo("\n[CARD EFFECT] Player " + targetP->getName() + " has moved to a new tile!");
                            }

                            try {
                                turnController.resolveTileLanding(&gameContext, targetP, economyController, effectController, auctionController, bankruptcyController, dice, saveLoader, inputHandler, logger, displayView);
                            } catch (const AuctionTriggerException&) {
                                auctionController.startAuctionSkipBuy(gameContext, displayView, inputHandler);
                            } catch (const BankruptcyException& ex) {
                                bankruptcyController.liquidateAssets(gameContext, *targetP, nullptr, ex.getRequired(), displayView, economyController, inputHandler, ex.getBankruptTile());
                            }

                            if (targetP == currentPlayer && (currentPlayer->getStatus() == PlayerStatus::JAILED || currentPlayer->getStatus() == PlayerStatus::BANKRUPT)) {
                                // Bankrupt controller handles assets and cards here.
                                turnEnded = true;
                            }
                        }
                    }
                    break; 
                }

                case CommandType::AKHIRI_GILIRAN: { 
                    if (!hasRolledDice) {
                        displayView.renderWarning("You have not rolled the dice yet! You must roll before ending your turn.");
                        break;
                    }
                    displayView.renderInfo("Ending turn...");
                    turnEnded = true;
                    break;
                }

                case CommandType::SIMPAN:
                    inputHandler.getStringInput();
                    saveLoader.saveGame(inputHandler.getLastStringInput(), gameContext, logger);
                    logger.addLog(gameContext.getCurrentTurn(), currentPlayer->getName(), "SIMPAN", inputHandler.getLastStringInput());
                    break;


                case CommandType::CETAK_LOG: {
                    int count = 0;
                    bool hasValue = false;
                    bool isInt = inputHandler.getIntRemaining(count, hasValue);

                    if (!hasValue) {
                        logger.printLogs(0);
                    } else if (isInt) {
                        logger.printLogs(count);
                    } else {
                        displayView.renderWarning("CETAK_LOG argument must be a number."); 
                    }
                    break;
                }
                case CommandType::HELP:{
                    displayView.showMenu();
                    break;
                }
                case CommandType::UNKNOWN_COMMAND:
                default:
                    displayView.renderWarning("Invalid command.");
                    break;

                }

            if (isDoubleRoll && currentPlayer->getStatus() == PlayerStatus::ACTIVE && currentPlayer->getJailTurns() == 0) {
                hasRolledDice = false;
                isDoubleRoll = false;
            }
        }

        effectController.decrementDurations();

        int activePlayers = count_if(gameContext.getPlayers().begin(), gameContext.getPlayers().end(), [](const Player& p) {
            return p.getStatus() != PlayerStatus::BANKRUPT;
        });

        if (gameContext.getMaxTurns() > 0 && gameContext.getCurrentTurn() > gameContext.getMaxTurns() || activePlayers == 1) {
            gameContext.setGameOver(true);
        } 
        else {
            gameContext.nextPlayer();
            if (gameContext.getCurrentPlayerIndex() == 0) {
                gameContext.setCurrentTurn(gameContext.getCurrentTurn() + 1);
            }
        }
    }
}