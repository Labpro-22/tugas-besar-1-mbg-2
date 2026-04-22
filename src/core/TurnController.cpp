#include "../include/core/TurnController.hpp"

Command TurnController::parseCommand(const string& cmdInput){
    static const map<string, Command> mapCommand = {
        {"LEMPAR_DADU", Command::LEMPAR_DADU},
        {"ATUR_DADU", Command::ATUR_DADU},
        {"CETAK_PAPAN", Command::CETAK_PAPAN},
        {"CETAK_AKTA", Command::CETAK_AKTA},
        {"CETAK_PROPERTI", Command::CETAK_PROPERTI},
        {"GADAI", Command::GADAI},
        {"TEBUS", Command::TEBUS},
        {"BANGUN", Command::BANGUN},
        {"GUNAKAN_KEMAMPUAN", Command::GUNAKAN_KEMAMPUAN},
        {"SIMPAN", Command::SIMPAN},
        {"MUAT", Command::MUAT},
        {"CETAK_LOG", Command::CETAK_LOG}
    };
    auto i = mapCommand.find(cmdInput);
    if (i != mapCommand.end())return i->second;
    return Command::UNKNOWN_COMMAND;
}

void TurnController::executeAction(GameContext* context, EconomyController& eco, EffectController& eff, AuctionController& auc, BankruptcyController& bank, Dice& dice, SaveLoader& sl){
    Player* currentPlayer = context->getCurrentPlayer();
    
    if (currentPlayer->getStatus() == PlayerStatus::BANKRUPT){
        context->nextPlayer();
        return;
    }
    cout << "Giliran: " << currentPlayer->getName() << " (Turn " << context->getCurrentTurn() << ")" << endl;

    bool hasRolledDice = false;
    
    if (currentPlayer->getStatus() == PlayerStatus::JAILED){
        int currentAttempt = currentPlayer->getJailTurns() + 1;
        currentPlayer->setJailTurns(currentAttempt);

        cout << "[DI DALAM PENJARA] Upaya ke-" << currentAttempt << endl;

        if (currentAttempt >= 4){
            cout <<"Anda sudah 3 giliran di penjara. Wajib membayar denda sebesar M"<< context->getJailFine() << "!" << endl;
            if(currentPlayer->canAfford(context->getJailFine())){
                *currentPlayer -= context->getJailFine();
                currentPlayer->setStatus(PlayerStatus::ACTIVE);
                currentPlayer->setJailTurns(0);
                cout << "Denda telah dibayar. Anda bebas!" << endl;
            } else {
                bank.declareBankruptcy(*currentPlayer);
                return;
            }
        } else {
            bool validJailAction = false;

            while (!validJailAction){
                cout << "Pilihan keluar penjara:" << endl;
                cout << "1. Bayar denda (M" << context->getJailFine() << ")" << endl;
                cout << "2. Gunakan kartu 'Bebas dari Penjara'" << endl;
                cout << "3. Kocok dadu (Harus Double)" << endl;
                cout << "Pilihan (1/2/3): ";

                int choice; 
                cin >> choice; 

                if (choice == 1){
                    if (currentPlayer->canAfford(context->getJailFine())) {
                        *currentPlayer -= context->getJailFine();
                        currentPlayer->setStatus(PlayerStatus::ACTIVE);
                        currentPlayer->setJailTurns(0);
                        cout << "Denda dibayar. Anda sekarang bebas!" << endl;
                    } else {
                        cout << "Uang tidak cukup!" << endl;
                }
            } else if (choice == 2){
                if (currentPlayer->hasSkillCard("jail gatau nanti dah")){
                    cout << "Menggunakan kartu bebas dari penjara..." << endl;
                    // currentPlayer->removeSkillCard(jail);
                    currentPlayer->setStatus(PlayerStatus::ACTIVE);
                    currentPlayer->setJailTurns(0);
                    validJailAction = true;
                } else {
                    cout << "GAGAL: Kamu tidak memiliki kartu 'Bebas dari Penjara'! Pilih opsi lain." << endl;
                }
            } else if (choice == 3){
                cout << "Mencoba mengocok dadu untuk double..." << endl;
                dice.roll();
                cout << "Hasil: " << dice.getDice1() << " + " << dice.getDice2() << " = " << dice.getTotal() << endl;
                if (dice.isDouble()) {
                    cout << "DOUBLE! Anda bebas dan langsung bergerak." << endl;
                    currentPlayer->setStatus(PlayerStatus::ACTIVE);
                    currentPlayer->setJailTurns(0);
                    hasRolledDice = true; 
                } else {
                    cout << "Gagal mendapatkan double. Anda tetap di penjara." << endl;
                    context->nextPlayer();
                    return; 
                }
                validJailAction = true;
            } else {
                cout << "Pilihat tidak valid!" << endl;
            }
        }
        }
    }
    while (!hasRolledDice && currentPlayer->getStatus() == PlayerStatus::ACTIVE){
        string input;
        cout << "\n[" << currentPlayer->getName() << "] Masukkan Perintah: ";
        cin >> input;

        Command cmd  = parseCommand(input);

        switch (cmd) { 
            case Command::LEMPAR_DADU : 
                cout << "Mengocok dadu..." << endl;
                dice.roll();
                hasRolledDice = true;
                break;
            case Command::ATUR_DADU : {
                int x, y; 
                cin >> x >> y;
                dice.setRoll(x,y); 
                hasRolledDice = true;
                break;
            }    
            case Command::CETAK_PAPAN : 
                // BELUM ADA IMPLEMENTASI VIEW NYA 
                break;
            case Command::CETAK_AKTA : {
                string code; 
                cout << "Masukkan kode petak: ";
                cin >> code; 
                // MANGGIL FUNGSI VIEW LAGI 
                break;
            }
            case Command::CETAK_PROPERTI :
                cout << "=== Properti Milik: " << currentPlayer->getName() << " ===" << endl;
                // MANGGIL FUNGSI YANG ADA DI VIEW LAGI
                break;
            case Command::GADAI: 
                // Belum ada implementasi handleGadai, harusnya di economycontroller
                break;
            case Command::TEBUS: 
                // Belum ada implementasi pembayaran gadai, harusnya di economycontroller
                break;
            case Command::BANGUN: 
                // Belum ada imeplementasi pembangunan, kemungkinan di economycontrolelre
                break; 
            case Command::GUNAKAN_KEMAMPUAN: 
                // Belum ada implementasi fungsi yang menangani ini, kemungkinan ada di effectCont
                break; 
            case Command::SIMPAN : {
                string filename; 
                cin >> filename;
                sl.saveGame(filename);
                break;
            }
            case Command::MUAT : {
                string filename; 
                cin >> filename; 
                sl.loadGame(filename);
                break;
            }
            case Command::CETAK_LOG : 
                // Belum ada implementasi fungsi yang menangani log
                break; 
            case Command::UNKNOWN_COMMAND : 
                // Ini mungkin pake exception? atau mau gimana
            default : 
                cout << "Perintah tidak valid" << endl;
                break;
        }
    }
    if (hasRolledDice) { 
        int diceTotal = dice.getTotal();
        cout << "Hasil: " << dice.getDice1() << " + " << dice.getDice2() << " = " << diceTotal << endl;
        cout << "Memajukan Bidak " << currentPlayer->getName() << " sebanyak " << diceTotal << " petak..." << endl;
        
        int oldPos = currentPlayer->getPosition();
        currentPlayer->move(diceTotal);
        
        if (currentPlayer->getPosition() < oldPos) {
            *currentPlayer += context->getGoSalary(); // Belum ada implementasi
            cout << "Melewati GO! Mendapat gaji M" << context->getGoSalary() << endl; // Belum ada implementasi 
        }

        Tile* currentTile = context->getBoard()->getTile(currentPlayer->getPosition());
        cout << "Bidak mendarat di " << currentTile->getName() << "." << endl;
        
        LandResult result = currentTile->land(*context);
        cout << ">> " << result.message << " <<" << endl;

        switch (result.type) {
            case LandEventType::OFFERBUYPROPERTY:
                // eco.handleOfferBuyProperty(result, context, auc, bank);
                break;

            case LandEventType::GIVEPROPERTY:
                // eco.handleGiveProperty(result, context);
                break;

            case LandEventType::PAYRENT:
                // eco.handlePayRent(result, context, bank, diceTotal);
                break;

            case LandEventType::OFFERUPGRADEPROPERTY:
                // bentar
                break;

            case LandEventType::PAYTAX:
                // eco.handlePayTax(result, context, bank);
                break;

            case LandEventType::DOFESTIVAL:
                // eff.handleFestival(result, context);
                break;

            case LandEventType::DRAWCARD:
                // eff.handleDrawCard(result, context);
                break;

            case LandEventType::GOTOJAIL:
                cout << "Anda mendarat di petak Go To Jail, Anda dipenjara!." << endl;
                currentPlayer->setStatus(PlayerStatus::JAILED);
                // currentPlayer->setPosition(11);  INI BELUM TAU PENJARA DI INDEKS BERAPA YA
                currentPlayer->setJailTurns(0);
                break;

            case LandEventType::BANKRUPTCYCHECK:

            case LandEventType::DONOTHING:

            case LandEventType::NONE:
                break;

            default:
                break;
        }
    }
    eff.decrementDurations(); // Kurangi durasi festival di akhir giliran

    if (dice.isDouble() && currentPlayer->getStatus() == PlayerStatus::ACTIVE && currentPlayer->getJailTurns() == 0) {
        cout << currentPlayer->getName() << " mendapat dadu double! Silakan main lagi." << endl;
    } else {
        context->nextPlayer();
    }
}