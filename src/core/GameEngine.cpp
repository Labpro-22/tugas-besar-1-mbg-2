#include "GameEngine.hpp"
#include "AuctionController.hpp"
#include "BankruptcyController.hpp"
#include "EffectController.hpp"
#include "DisplayView.hpp"
#include "InputHandler.hpp"
#include "GameLogger.hpp"

using namespace std;

void GameEngine::initGame(GameContext& gameContext, TurnController& turnController, ConfigReader& configReader, EconomyController& economyController) {
    configReader.loadAllConfigs(&gameContext, gameContext.getBoard(), economyController, turnController);

    CardDeck<ActionCard>& chanceDeck = gameContext.getChanceDeck();
    chanceDeck.addCard(new MoveToStationCard());
    chanceDeck.addCard(new MoveBackwardCard());
    chanceDeck.addCard(new MoveToJailCard());
    chanceDeck.initShuffle();

    CardDeck<ActionCard>& comChestDeck = gameContext.getCommunityChestDeck();
    comChestDeck.addCard(new BirthdayCard());
    comChestDeck.addCard(new DoctorFeeCard());
    comChestDeck.addCard(new NyalegCard());
    comChestDeck.initShuffle();

    CardDeck<SkillCard>& skillDeck = gameContext.getSkillDeck();
    for (int i = 0; i < 4; ++i) {
        skillDeck.addCard(new MoveCard());
    }

    for (int i = 0; i < 3; ++i) {
        skillDeck.addCard(new DiscountCard()); 
    }

    for (int i = 0; i < 2; ++i) {
        skillDeck.addCard(new ShieldCard());
    }

    for (int i = 0; i < 2; ++i) {
        skillDeck.addCard(new TeleportCard());
    }

    for (int i = 0; i < 2; ++i) {
        skillDeck.addCard(new LassoCard());
    }

    for (int i = 0; i < 2; ++i) {
        skillDeck.addCard(new DemolitionCard());
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

    if (gameContext.getPlayers().empty()) {
        cout << "Daftar pemain kosong. Tambahkan pemain sebelum game loop dijalankan." << endl;
        return; 
    }

    while (!gameContext.isGameOver()) {
        Player* currentPlayer = &gameContext.getCurrentPlayer();
        
        if (currentPlayer->getStatus() == PlayerStatus::BANKRUPT) {
            gameContext.nextPlayer();
            continue;
        }

        cout << "\n============================================\n";
        cout << "Giliran: " << currentPlayer->getName() << " (Turn " << gameContext.getCurrentTurn() << ")" << endl;
        bool turnEnded = false;

        if (currentPlayer->getStatus() == PlayerStatus::JAILED) {
            int currentAttempt = currentPlayer->getJailTurns() + 1;
            currentPlayer->setJailTurns(currentAttempt);

            cout << "[DI DALAM PENJARA] Upaya ke-" << currentAttempt << endl;

            if (currentAttempt >= 4) {
                cout <<"Anda sudah 3 giliran di penjara. Wajib membayar denda sebesar M"<< gameContext.getJailFine() << "!" << endl;
                if(currentPlayer->canAfford(gameContext.getJailFine())){
                    *currentPlayer -= gameContext.getJailFine();
                    currentPlayer->setStatus(PlayerStatus::ACTIVE);
                    currentPlayer->setJailTurns(0);
                    cout << "Denda telah dibayar. Anda bebas!" << endl;
                } else {
                    bankruptcyController.declareBankruptcy(*currentPlayer);
                    turnEnded = true; 
                }
            } else {
                bool validJailAction = false;

                while (!validJailAction) {
                    cout << "Pilihan keluar penjara:" << endl;
                    cout << "1. Bayar denda (M" << gameContext.getJailFine() << ")" << endl;
                    cout << "2. Gunakan kartu 'Bebas dari Penjara'" << endl;
                    cout << "3. Kocok dadu (Harus Double)" << endl;
                    cout << "Pilihan (1/2/3): ";

                    inputHandler.getIntInput();
                    int choice = inputHandler.getIntValue1(); 

                    if (choice == 1) {
                        if (currentPlayer->canAfford(gameContext.getJailFine())) {
                            *currentPlayer -= gameContext.getJailFine();
                            currentPlayer->setStatus(PlayerStatus::ACTIVE);
                            currentPlayer->setJailTurns(0);
                            cout << "Denda dibayar. Anda sekarang bebas!" << endl;
                            validJailAction = true;
                        } else {
                            cout << "Uang tidak cukup!" << endl;
                        }
                    } else if (choice == 2) {
                        if (currentPlayer->hasSkillCard("BEBAS_PENJARA")) {
                            cout << "Menggunakan kartu bebas dari penjara..." << endl;
                            // currentPlayer->removeSkillCard("BEBAS_PENJARA"); TAR DIURUS MIKA SEMOGA
                            currentPlayer->setStatus(PlayerStatus::ACTIVE);
                            currentPlayer->setJailTurns(0);
                            validJailAction = true;
                        } else {
                            cout << "GAGAL: Kamu tidak memiliki kartu 'Bebas dari Penjara'! Pilih opsi lain." << endl;
                        }
                    } else if (choice == 3) {
                        cout << "Mencoba mengocok dadu untuk double..." << endl;
                        dice.roll();
                        cout << "Hasil: " << dice.getDice1() << " + " << dice.getDice2() << " = " << dice.getTotal() << endl;
                        if (dice.isDouble()) {
                            cout << "DOUBLE! Anda bebas dan langsung bergerak." << endl; // ini gw asumsi kalau double langsung keluar dengan jumlah double itu ya
                            currentPlayer->setStatus(PlayerStatus::ACTIVE);
                            currentPlayer->setJailTurns(0);
                            turnController.executeAction(&gameContext, economyController, effectController, auctionController, bankruptcyController, dice, saveLoader, inputHandler, logger);
                        } else {
                            cout << "Gagal mendapatkan double. Anda tetap di penjara." << endl;
                            gameContext.nextPlayer();
                        }
                        validJailAction = true;
                        turnEnded = true; 
                    } else {
                        cout << "Pilihan tidak valid!" << endl;
                        inputHandler.clearInputBuffer();
                    }
                }
            }
        }

        while (!turnEnded && currentPlayer->getStatus() == PlayerStatus::ACTIVE) {
            cout << "\n[" << currentPlayer->getName() << "] Masukkan Perintah: ";
            CommandType cmd = inputHandler.getCommand();

            switch (cmd) {
                case CommandType::LEMPAR_DADU:
                    dice.roll();
                    turnController.executeAction(&gameContext, economyController, effectController, auctionController, bankruptcyController, dice, saveLoader, inputHandler, logger);
                    turnEnded = true;
                    break;

                case CommandType::ATUR_DADU: {
                    inputHandler.getIntTwoInput();
                    int x = inputHandler.getIntValue1();
                    int y = inputHandler.getIntValue2();
                    dice.setRoll(x, y);
                    turnController.executeAction(&gameContext, economyController, effectController, auctionController, bankruptcyController, dice, saveLoader, inputHandler, logger);
                    turnEnded = true;
                    break;
                }

                case CommandType::CETAK_PAPAN:
                    displayView.renderBoard(gameContext);
                    break;

                case CommandType::CETAK_AKTA: {
                    cout << "Masukkan kode petak: ";
                    inputHandler.getStringInput();
                    displayView.renderAkta(gameContext, inputHandler.getLastStringInput());
                    break;
                }

                case CommandType::CETAK_PROPERTI:
                    displayView.renderProperty(gameContext);
                    break;

                case CommandType::GADAI:
                    // BELUM ADA IMPLEMENTASI
                case CommandType::TEBUS:
                    // BELUM ADA IMPLEMENTASI
                case CommandType::BANGUN:
                    // BELUM ADA IMPLEMENTASI
                case CommandType::GUNAKAN_KEMAMPUAN:
                    // INI NUNGGU MIKA KAYAKNYA
                    break;

                case CommandType::SIMPAN:
                    inputHandler.getStringInput();
                    saveLoader.saveGame(inputHandler.getLastStringInput(), gameContext);
                    logger.addLog(gameContext.getCurrentTurn(), currentPlayer->getName(), "SIMPAN", inputHandler.getLastStringInput());
                    break;

                case CommandType::MUAT:
                    cout << "Masukkan nama file save: ";
                    inputHandler.getStringInput();
                    saveLoader.loadGame(inputHandler.getLastStringInput(), gameContext);
                    logger.addLog(gameContext.getCurrentTurn(), currentPlayer->getName(), "MUAT", inputHandler.getLastStringInput());
                    break;

                case CommandType::CETAK_LOG:
                {
                    int count = 0;
                    bool hasValue = false;
                    bool isInt = inputHandler.getIntRemaining(count, hasValue);

                    if (!hasValue) {
                        logger.printLogs(0);
                    } else if (isInt) {
                        logger.printLogs(count);
                    } else {
                        cout << "Argumen CETAK_LOG harus angka." << endl; // gw rasa ini nanti bisa dibuat exception kalau mau
                    }
                    break;
                }

                case CommandType::UNKNOWN_COMMAND:
                default:
                    cout << "Perintah tidak valid." << endl;
                    break;
            }
        }

        if (gameContext.getMaxTurns() > 0 && gameContext.getCurrentTurn() >= gameContext.getMaxTurns()) {
            gameContext.setGameOver(true);
        } else {
            gameContext.setCurrentTurn(gameContext.getCurrentTurn() + 1);
        }
    }
}
