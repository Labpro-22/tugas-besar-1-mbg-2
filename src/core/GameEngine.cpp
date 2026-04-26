#include "GameEngine.hpp"
#include "AuctionController.hpp"
#include "BankruptcyController.hpp"
#include "EffectController.hpp"
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
    Dice dice;

    initGame(gameContext, turnController, configReader, economyController);

    if (isGUIMode && guiView != nullptr) {
        cliView.setLogSink([this](const std::string& message) {
            if (this->guiView != nullptr) {
                this->guiView->addLog(message);
            }
        });
    } else {
        cliView.clearLogSink();
    }

    int startingIndex = 0;
    bool gameReady = false;

    while (!gameReady) {
        cliView.renderStart();

        inputHandler.getIntInput();
        int menuChoice = inputHandler.getIntValue1();

        if (menuChoice == 1) {
            cliView.renderInfo("\n--- NEW GAME ---\n");
            int numPlayers = 0;
            while (numPlayers < 2 || numPlayers > 4) { 
                cliView.renderInfo("Input number of players (2-4): ");
                inputHandler.getIntInput();
                numPlayers = inputHandler.getIntValue1();
                if (numPlayers < 2 || numPlayers > 4) {
                    cliView.renderInfo("Number of players must be 2, 3, or 4!");
                }
            }

            for (int i = 0; i < numPlayers; ++i) {
                string pName;
                bool isUnique = false;

                while (!isUnique) {
                    cliView.renderInfo("Input name of Player " + to_string(i + 1) + ": ");
                    inputHandler.getStringInput();
                    pName = inputHandler.getLastStringInput();

                    isUnique = true;

                    if (pName == "" || pName == "BANK") {
                        cliView.renderWarning("[ERROR] Invalid name! You cannot use an empty name or 'BANK'.");
                        isUnique = false;
                        continue;
                    }

                    for (const Player& p : gameContext.getPlayers()) {
                        if (p.getName() == pName) {
                            cliView.renderWarning("[ERROR] Name '" + pName + "' is already taken! Please choose a different name.");
                            isUnique = false;
                            break;
                        }
                    }
                }
                
                Player newPlayer(pName);
                newPlayer.setBalance(gameContext.getStartingMoney()); 
                gameContext.getPlayers().push_back(newPlayer);
            }
            
            srand(static_cast<unsigned>(time(0)));
            startingIndex = rand() % numPlayers;

            gameContext.setCurrentPlayerIndex(startingIndex);
            
            cliView.renderInfo("\n[RANDOMIZING STARTING PLAYER]...");
            cliView.renderInfo("\n>> Game will start with: " + gameContext.getPlayers()[startingIndex].getName() + "!\n");
            gameReady = true;

        } else if (menuChoice == 2) {
            cliView.renderInfo("\n--- LOAD GAME ---\n");
            cliView.renderInfo("Input save file name: ");
            inputHandler.getStringInput();
            string saveFile = inputHandler.getLastStringInput();
            
            saveLoader.loadGame(saveFile, gameContext, logger); 
            
            if (!gameContext.getPlayers().empty()) {
                cliView.renderInfo("\n[SUCCESS] Load successful!\n");
                gameReady = true;
            } else {
                cliView.renderInfo("\n[ERROR] Failed to load save file or data is empty. Please try again.\n");
            }
        } else {
            cliView.renderInfo("\n[ERROR] Invalid choice!\n");
            inputHandler.clearInputBuffer();
        }
    }

    if (gameContext.getPlayers().empty()) {
        cliView.renderInfo("Player list is empty. Add players before running the game loop.");
        return; 
    }

    Player* previousPlayer = nullptr;
    bool hasUsedSkillThisTurn = false;

    while (!gameContext.isGameOver()) {
        
        if (gameContext.getCurrentPlayerIndex() == 0) {
            turnController.distributeSkillCards(gameContext, inputHandler, cliView);
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

        cliView.renderInfo("============================================");
        cliView.renderInfo("Turn: " + currentPlayer->getName() + " (Round " + to_string(gameContext.getCurrentTurn()) + ")");
        if (isGUIMode && guiView != nullptr) {
            guiView->updateBoardState(gameContext);
        }
        
        bool turnEnded = false;
        bool isDoubleRoll = false;
        bool hasRolledDice = false;

        if (currentPlayer->getStatus() == PlayerStatus::JAILED) {
            int currentAttempt = currentPlayer->getJailTurns() + 1;
            currentPlayer->setJailTurns(currentAttempt);

            cliView.renderInfo("[IN JAIL] Attempt #" + to_string(currentAttempt));

            if (currentAttempt >= 4) {
                cliView.renderInfo("You have spent 3 turns in jail. You must pay a fine of M" + to_string(gameContext.getJailFine()) + "!");
                try {
                    *currentPlayer -= gameContext.getJailFine();
                    currentPlayer->setStatus(PlayerStatus::ACTIVE);
                    currentPlayer->setJailTurns(0);
                    cliView.renderInfo("Fine paid. You are free now!");
                } catch (const InsufficientFundsException& ex) {
                    bankruptcyController.liquidateAssets(gameContext, *currentPlayer, nullptr, ex.getRequired(), cliView, economyController, inputHandler, nullptr);
                    turnEnded = true;
                }
            } else {
                bool validJailAction = false;

                while (!validJailAction) {
                    cliView.renderInfo("Jail escape options:");
                    cliView.renderInfo("1. Pay fine (M" + to_string(gameContext.getJailFine()) + ")");
                    cliView.renderInfo("2. Use 'Get Out of Jail' card");
                    cliView.renderInfo("3. Roll dice (must be doubles)");
                    cliView.renderPrompt("Choice (1/2/3): ");

                    inputHandler.getIntInput();
                    int choice = inputHandler.getIntValue1(); 

                    if (choice == 1) {
                        try {
                            *currentPlayer -= gameContext.getJailFine();
                            currentPlayer->setStatus(PlayerStatus::ACTIVE);
                            currentPlayer->setJailTurns(0);
                            cliView.renderInfo("Fine paid. You are now free!");
                            validJailAction = true;
                        } catch (const InsufficientFundsException& ex) {
                            cliView.renderWarning("Insufficient funds!");
                            bankruptcyController.liquidateAssets(gameContext, *currentPlayer, nullptr, ex.getRequired(), cliView, economyController, inputHandler, nullptr);
                            turnEnded = true;
                            validJailAction = true;
                        }
                    } else if (choice == 2) {
                        int jailFreeIdx = currentPlayer->searchJailFreeCard();
                        
                        if (jailFreeIdx != -1) {
                            cliView.renderInfo("Using 'Get Out of Jail' card...");
                            
                            SkillCard* usedCard = currentPlayer->useSkillCard(jailFreeIdx);
                            
                            if (usedCard != nullptr) {
                                gameContext.getSkillDeck().discard(usedCard);
                            }

                            effectController.execute(*usedCard, *currentPlayer, gameContext, inputHandler, cliView);
                            validJailAction = true;
                        } else {
                            cliView.renderWarning("FAILED: You do not have a 'Get Out of Jail' card! Choose another option.");
                        }
                    } else if (choice == 3) {
                        cliView.renderInfo("Attempting to roll doubles...");
                        dice.roll();
                        cliView.renderInfo("Result: " + to_string(dice.getDice1()) + " + " + to_string(dice.getDice2()) + " = " + to_string(dice.getTotal()));
                        if (dice.isDouble()) {
                            cliView.renderInfo("DOUBLE! You are free and move immediately."); 
                            currentPlayer->setStatus(PlayerStatus::ACTIVE);
                            currentPlayer->setJailTurns(0);
                            try {
                                turnController.handleDiceRollMovement(&gameContext, economyController, effectController, auctionController, bankruptcyController, dice, saveLoader, inputHandler, logger, cliView, isGUIMode, guiView);
                                if (isGUIMode && guiView != nullptr) {
                                    guiView->updateBoardState(gameContext);
                                }
                            } catch (const AuctionTriggerException&) {
                                auctionController.startAuctionSkipBuy(gameContext, cliView, inputHandler);
                                turnEnded = true;
                            } catch (const BankruptcyException& ex) {
                                bankruptcyController.liquidateAssets(gameContext, *currentPlayer, nullptr, ex.getRequired(), cliView, economyController, inputHandler, ex.getBankruptTile());
                                turnEnded = true;
                            }
                            hasRolledDice = true;
                        } else {
                            cliView.renderInfo("Failed to roll doubles. You remain in jail.");
                            turnEnded = true;
                        }
                        validJailAction = true;
                    } else {
                        cliView.renderWarning("Invalid choice!");
                        inputHandler.clearInputBuffer();
                    }
                }
            }
        }

        while (!turnEnded && currentPlayer->getStatus() == PlayerStatus::ACTIVE) {
            // cliView.renderPrompt("\n[" + currentPlayer->getName() + "] Enter Command: ");
            // CommandType cmd = inputHandler.getCommand();
            CommandType cmd;
            
            if (isGUIMode && guiView != nullptr) {
                cmd = guiView->getGUICommand(); 
            } 
            else {
                cliView.renderPrompt("\n[" + currentPlayer->getName() + "] Enter Command: ");
                cmd = inputHandler.getCommand(); 
            }

            switch (cmd) {
                case CommandType::LEMPAR_DADU:
                    if (hasRolledDice) {
                        cliView.renderWarning("You have already rolled the dice this turn! Complete another action or end your turn.");
                        break;
                    }
                    dice.roll();
                    gameContext.getDice().setRoll(dice.getDice1(), dice.getDice2());
                    if (dice.isDouble()) {
                        int currentDoubleCount = currentPlayer->getDoubleCount() + 1;
                        currentPlayer->setDoubleCount(currentDoubleCount);

                        if (currentDoubleCount == 3) {
                            cliView.renderInfo("\n*** 3 DOUBLES IN A ROW! GO TO JAIL! ***\n");
                            if (currentPlayer->hasShield()) {
                                cliView.renderInfo("[SHIELD ACTIVE] You are protected! You safely escaped from going to Jail.");
                            } 
                            else {
                                cliView.renderInfo("YOU ARE SENT TO JAIL!");
                                Tile* jailTile = gameContext.getBoard().getTileByCode("PEN");
                                
                                if (jailTile != nullptr) {
                                    int jailPos = jailTile->getIdx();
                                    currentPlayer->setPosition(jailPos);
                                    currentPlayer->setStatus(PlayerStatus::JAILED); 
                                    currentPlayer->setJailTurns(0);
                                }
                            }
                            currentPlayer->setDoubleCount(0);
                            hasRolledDice = true;
                            turnEnded = true; 
                            break;
                        } 
                        else {
                            isDoubleRoll = true;
                            cliView.renderInfo("\n" + currentPlayer->getName() + " rolled DOUBLES! Gets an extra turn! (Double count: " + to_string(currentDoubleCount) + ")");
                        }
                    }
                    else {
                        currentPlayer->setDoubleCount(0);
                    }
                    
                    try {
                        cliView.renderDiceRoll(gameContext, dice);
                        if (isGUIMode && guiView != nullptr) {
                            guiView->showDiceAnimation(dice.getDice1(), dice.getDice2());
                        }
                        turnController.handleDiceRollMovement(&gameContext, economyController, effectController, auctionController, bankruptcyController, dice, saveLoader, inputHandler, logger, cliView, isGUIMode, guiView);
                        if (isGUIMode && guiView != nullptr) {
                            guiView->updateBoardState(gameContext);
                        }
                    } catch (const AuctionTriggerException&) {
                        auctionController.startAuctionSkipBuy(gameContext, cliView, inputHandler);
                        if (isGUIMode && guiView != nullptr) guiView->updateBoardState(gameContext);
                    } catch (const BankruptcyException& ex) {
                        bankruptcyController.liquidateAssets(gameContext, *currentPlayer, nullptr, ex.getRequired(), cliView, economyController, inputHandler, ex.getBankruptTile());
                        if (isGUIMode && guiView != nullptr) guiView->updateBoardState(gameContext);
                    }
                    hasRolledDice = true;
                    break;

                case CommandType::ATUR_DADU: {
                    if (hasRolledDice) {
                        cliView.renderWarning("You have already rolled the dice this turn!");
                        break;
                    }
                    inputHandler.getIntTwoInput();
                    int x = inputHandler.getIntValue1();
                    int y = inputHandler.getIntValue2();
                    dice.setRoll(x, y);
                    gameContext.getDice().setRoll(dice.getDice1(), dice.getDice2());
                    if (dice.isDouble()) {
                        int currentDoubleCount = currentPlayer->getDoubleCount() + 1;
                        currentPlayer->setDoubleCount(currentDoubleCount);

                        if (currentDoubleCount == 3) {
                            cliView.renderInfo("\n*** 3 DOUBLES IN A ROW! ***");
                            
                            if (currentPlayer->hasShield()) {
                                cliView.renderInfo("[SHIELD ACTIVE] You are protected! You safely escaped from going to Jail.");
                            } 
                            else {
                                cliView.renderInfo("YOU ARE SENT TO JAIL!");
                                Tile* jailTile = gameContext.getBoard().getTileByCode("PEN");
                                
                                if (jailTile != nullptr) {
                                    int jailPos = jailTile->getIdx();
                                    currentPlayer->setPosition(jailPos);
                                    currentPlayer->setStatus(PlayerStatus::JAILED); 
                                    currentPlayer->setJailTurns(0);
                                }
                            }
                            
                            currentPlayer->setDoubleCount(0);
                            hasRolledDice = true;
                            turnEnded = true;
                            break;
                        }
                        else {
                            isDoubleRoll = true;
                            cliView.renderInfo("\n" + currentPlayer->getName() + " set DOUBLES! Gets an extra turn! (Double count: " + to_string(currentDoubleCount) + ")\n");
                        }
                    }
                    else {
                        currentPlayer->setDoubleCount(0);
                    }
                    try {
                        cliView.renderDiceRoll(gameContext, dice);
                        if (isGUIMode && guiView != nullptr) {
                            guiView->showDiceAnimation(dice.getDice1(), dice.getDice2());
                        }
                        turnController.handleDiceRollMovement(&gameContext, economyController, effectController, auctionController, bankruptcyController, dice, saveLoader, inputHandler, logger, cliView, isGUIMode, guiView);
                        if (isGUIMode && guiView != nullptr) guiView->updateBoardState(gameContext);
                    } catch (const AuctionTriggerException&) {
                        auctionController.startAuctionSkipBuy(gameContext, cliView, inputHandler);
                        if (isGUIMode && guiView != nullptr) guiView->updateBoardState(gameContext);
                    } catch (const BankruptcyException& ex) {
                        bankruptcyController.liquidateAssets(gameContext, *currentPlayer, ex.getCreditor(), ex.getRequired(), cliView, economyController, inputHandler, ex.getBankruptTile());
                        if (isGUIMode && guiView != nullptr) guiView->updateBoardState(gameContext);
                    }
                    hasRolledDice = true;
                    break;
                }

                case CommandType::CETAK_PAPAN:
                    cliView.renderBoard(gameContext);

                    if (isGUIMode && guiView != nullptr) {
                        guiView->updateBoardState(gameContext);
                    }

                    break;

                case CommandType::CETAK_AKTA: {
                    inputHandler.getStringInput();
                    cliView.renderAkta(gameContext, inputHandler.getLastStringInput());
                    break;
                }

                case CommandType::CETAK_PROPERTI:
                    cliView.renderProperty(gameContext);
                    break;

                case CommandType::GADAI: {
                    vector<PropertyTile*> mortgageProperty = currentPlayer->getUnmortgagedProperties();
                    cliView.renderMortgageStart(gameContext, mortgageProperty);

                    if (mortgageProperty.empty()){
                        cliView.renderInfo("You have no properties available to mortgage!");
                        break;
                    }

                    inputHandler.getIntInput();
                    int choice = inputHandler.getIntValue1();

                    if (choice == 0) {
                        cliView.renderInfo("Mortgage cancelled.");
                        break;
                    }
                    if (choice < 1 || choice >( int )mortgageProperty.size()) {
                        cliView.renderWarning("Invalid choice! Mortgage cancelled.");
                        break;
                    }
                    PropertyTile* tile = mortgageProperty[choice - 1];

                    if (auto* railroadTile = dynamic_cast<RailroadTile*>(tile)) {
                        economyController.mortgageProperty( *currentPlayer, railroadTile );
                        cliView.renderMortgageResult( gameContext, mortgageProperty[choice - 1] );
                        break;
                    }

                    if (auto* utilityTile = dynamic_cast<UtilityTile*>(tile)) {
                        economyController.mortgageProperty( *currentPlayer, utilityTile );
                        cliView.renderMortgageResult( gameContext, mortgageProperty[choice - 1] );
                        break;
                    }

                    if (auto* streetTile = dynamic_cast<StreetTile*>(tile)) {
                        vector<StreetTile*> sameColorTiles = economyController.getColorGroupTiles( &gameContext, streetTile->getColor() );
                        bool hasBuildingInGroup = false;

                        for (StreetTile* street : sameColorTiles) {
                            if (street->getOwner() == currentPlayer && (street->getHouseCount() > 0 || street->getHasHotel())) {
                                hasBuildingInGroup = true;
                                break;
                            }
                        }
                        if (hasBuildingInGroup) {
                            inputHandler.getStringInput();
                            string sellChoice = inputHandler.getLastStringInput();

                            cliView.renderMortgageGroupColorStart(gameContext, sameColorTiles);
                            cliView.renderMortgageGroupColorResult(gameContext, sellChoice, sameColorTiles);
                            if (sellChoice != "y" && sellChoice != "Y") {
                                break;
                            }
                            economyController.sellAllBuildingsInColorGroup(&gameContext, *currentPlayer,streetTile->getColor());
                            
                            cliView.renderPrompt("Continue to mortgage " + streetTile->getName() + "? (y/n): "); 
                            inputHandler.getStringInput();
                            string continueChoice = inputHandler.getLastStringInput();

                            if (continueChoice != "y" && continueChoice != "Y") {
                                cliView.renderInfo("Mortgage cancelled.");
                                break;
                            }
                        }
                        cliView.renderMortgageResult( gameContext, mortgageProperty[choice - 1] );
                        economyController.mortgageProperty( *currentPlayer, tile );
                    
                    }
                    
                    break;
                }
                case CommandType::TEBUS:{
                    vector<PropertyTile*> mortgageProperty = currentPlayer->getMortgagedProperties();

                    if (mortgageProperty.empty()){
                        cliView.renderInfo("You have no properties available to redeem!");
                        break;  
                    }

                    cliView.renderRedeemStart(gameContext, mortgageProperty);   
                    inputHandler.getIntInput();
                    int choice = inputHandler.getIntValue1();
                    while (choice < 0 || choice > mortgageProperty.size()){
                        if (choice == 0){
                            break;
                        }
                        inputHandler.getIntInput();
                        choice = inputHandler.getIntValue1();
                        cliView.renderRedeemChoose(gameContext, mortgageProperty, choice, 0);   
                    }
                    
                    
                    if (choice == 0) {
                        cliView.renderRedeemChoose(gameContext, mortgageProperty, choice, 0);   
                        break;
                    }
                    PropertyTile* selected = mortgageProperty[choice - 1];
                    try {
                        int redeemPrice = selected->getPrice();
                        *currentPlayer -= redeemPrice;
                        selected->setStatus( OWNED );
                        cliView.renderRedeemChoose(gameContext, mortgageProperty, choice, redeemPrice);
                    }
                    catch (const InsufficientFundsException& ex) {
                        cliView.renderInfo("You don't have enough balance to redeem this property.");
                        cliView.renderInfo("Required: M" + to_string(ex.getRequired()) + "| Your Balance: M" + to_string(currentPlayer->getBalance()));
                        break;
                    }
                    break;
                }   
                case CommandType::BANGUN:
                    turnController.handleBuildHouse(&gameContext, currentPlayer, economyController, inputHandler, cliView);
                    if (isGUIMode && guiView != nullptr) guiView->updateBoardState(gameContext);
                    break;

                case CommandType::GUNAKAN_KEMAMPUAN: {
                    if (hasRolledDice) {
                        cliView.renderWarning("Skill cards can ONLY be used BEFORE rolling the dice!");
                        break;
                    }

                    if (hasUsedSkillThisTurn) {
                        cliView.renderWarning("You can use a MAXIMUM of 1 Skill Card per turn!");
                        break;
                    }

                    if (!currentPlayer->hasAnySkillCard()) {
                        cliView.renderInfo("[ERROR] You do not have any Skill Cards!");
                        break;
                    }

                    cliView.renderInfo("Your Skill Cards:");
                    int displayIdx = 1;
                    for (SkillCard* card : currentPlayer->getSkillCard()) {
                        cliView.renderInfo("[" + to_string(displayIdx) + "] " + card->getName() + " - " + card->getDescription());
                        displayIdx++;
                    }
                    cliView.renderInfo("[0] Cancel card usage.");
                    cliView.renderPrompt("Choose a card to use (0-" + to_string(currentPlayer->getSkillCardCount()) + "): ");

                    int choice = -1;
                    while (true) {
                        inputHandler.getIntInput();
                        choice = inputHandler.getIntValue1();

                        if (choice >= 0 && choice <= currentPlayer->getSkillCardCount()) {
                            break;
                        }
                        cliView.renderPrompt("Invalid choice! Enter a number (0-" + to_string(currentPlayer->getSkillCardCount()) + "): ");
                    }

                    if (choice == 0) {
                        cliView.renderInfo("Skill card usage canceled.");
                        break;
                    }

                    int vectorIndex = choice - 1;
                    SkillCard* cardToUse = currentPlayer->dropSkillCard(vectorIndex);

                    cliView.renderInfo("\n>> Activating card: [" + cardToUse->getName() + "]...");

                    vector<int> preSkillPositions;
                    for (Player& p : gameContext.getPlayers()) {
                        preSkillPositions.push_back(p.getPosition());
                    }

                    effectController.execute(*cardToUse, *currentPlayer, gameContext, inputHandler, cliView);
                    gameContext.getSkillDeck().discard(cardToUse);
                    hasUsedSkillThisTurn = true;

                    for (int i = 0; i < gameContext.getPlayers().size(); i++) {
                        Player* targetP = &gameContext.getPlayers()[i];
                        if (targetP->getStatus() == PlayerStatus::BANKRUPT) continue;

                        if (targetP->getPosition() != preSkillPositions[i]) {
                            if (targetP == currentPlayer) {
                                cliView.renderInfo("\n[CARD EFFECT] You have moved to a new tile!");
                            } else {
                                cliView.renderInfo("\n[CARD EFFECT] Player " + targetP->getName() + " has moved to a new tile!");
                            }

                            try {
                                turnController.resolveTileLanding(&gameContext, targetP, economyController, effectController, auctionController, bankruptcyController, dice, saveLoader, inputHandler, logger, cliView, isGUIMode, guiView);
                            } catch (const AuctionTriggerException&) {
                                auctionController.startAuctionSkipBuy(gameContext, cliView, inputHandler);
                            } catch (const BankruptcyException& ex) {
                                bankruptcyController.liquidateAssets(gameContext, *targetP, nullptr, ex.getRequired(), cliView, economyController, inputHandler, ex.getBankruptTile());
                            }

                            if (targetP == currentPlayer && (currentPlayer->getStatus() == PlayerStatus::JAILED || currentPlayer->getStatus() == PlayerStatus::BANKRUPT)) {
                                // Bankrupt controller handles assets and cards here.
                                turnEnded = true;
                            }
                        }
                    }

                    if (isGUIMode && guiView != nullptr) guiView->updateBoardState(gameContext);
                    break; 
                }

                case CommandType::AKHIRI_GILIRAN: { 
                    if (!hasRolledDice) {
                        cliView.renderWarning("You have not rolled the dice yet! You must roll before ending your turn.");
                        break;
                    }
                    cliView.renderInfo("Ending turn...");
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
                        cliView.renderWarning("CETAK_LOG argument must be a number."); 
                    }
                    break;
                }
                case CommandType::HELP:{
                    cliView.showMenu();
                    break;
                }
                case CommandType::UNKNOWN_COMMAND:
                default:
                    cliView.renderWarning("Invalid command.");
                    break;

                }

            if (isDoubleRoll && currentPlayer->getStatus() == PlayerStatus::ACTIVE && currentPlayer->getJailTurns() == 0) {
                hasRolledDice = false;
                isDoubleRoll = false;
            }
        }

        effectController.decrementDurations(&gameContext);

        int activePlayers = gameContext.countActivePlayers();

        if (gameContext.getMaxTurns() > 0 && gameContext.getCurrentTurn() > gameContext.getMaxTurns() || activePlayers == 1) {
            gameContext.setGameOver(true);
        } 
        else {
            currentPlayer->setDoubleCount(0);
            gameContext.nextPlayer();
            if (gameContext.getCurrentPlayerIndex() == startingIndex) {
                gameContext.setCurrentTurn(gameContext.getCurrentTurn() + 1);
            }
        }
    }

    int finalActivePlayers = gameContext.countActivePlayers();

    if (finalActivePlayers == 1) {
        Player* winner = nullptr;
        for (Player& p : gameContext.getPlayers()) {
            if (p.getStatus() != PlayerStatus::BANKRUPT) {
                winner = &p;
                break;
            }
        }
        cliView.renderGameOverBankruptcy(winner);
    }
    else {
        vector<Player*> remainingPlayers;
        for (Player& p : gameContext.getPlayers()) {
            if (p.getStatus() != PlayerStatus::BANKRUPT) {
                remainingPlayers.push_back(&p);
            }
        }

        sort(remainingPlayers.begin(), remainingPlayers.end(), [](Player* a, Player* b) {
            if (a->getBalance() != b->getBalance()) {
                return a->getBalance() > b->getBalance();
            }
            if (a->getOwnedProperties().size() != b->getOwnedProperties().size()) {
                return a->getOwnedProperties().size() > b->getOwnedProperties().size();
            }
            return a->getSkillCardCount() > b->getSkillCardCount();
        });

        vector<Player*> winners;
        winners.push_back(remainingPlayers[0]);
        for (int i = 1; i < remainingPlayers.size(); ++i) {
            if (remainingPlayers[i]->getBalance() == winners[0]->getBalance() &&
                remainingPlayers[i]->getOwnedProperties().size() == winners[0]->getOwnedProperties().size() &&
                remainingPlayers[i]->getSkillCardCount() == winners[0]->getSkillCardCount()) {
                
                winners.push_back(remainingPlayers[i]);
            } 
            else {
                break;
            }
        }
        cliView.renderGameOverMaxTurn(remainingPlayers, winners);
    }
}