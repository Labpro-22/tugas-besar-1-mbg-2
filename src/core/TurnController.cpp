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

void TurnController::executeAction(GameContext* context, EconomyController& eco, EffectController& eff, AuctionController& auc, BankruptcyController& bank){
    Player* currentPlayer = context->getCurrentPlayer();
    
    if (currentPlayer->getStatus() == PlayerStatus::BANKRUPT){
        context->nextPlayer();
        return;
    }
    cout << "Giliran: " << currentPlayer->getName() << " (Turn " << context->getCurrentTurn() << ")" << endl;

    bool hasRolledDice = false;
    Dice dadu;
    
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
                dadu.roll();
                cout << "Hasil: " << dadu.getDice1() << " + " << dadu.getDice2() << " = " << dadu.getTotal() << endl;
                if (dadu.isDouble()) {
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
                dadu.roll();
                hasRolledDice = true;
                break;
            case Command::ATUR_DADU : 
                int x, y; 
                cin >> x >> y;
                dadu.setRoll(x,y); 
                hasRolledDice = true;
                break;
            case Command::CETAK_PAPAN : 
                // BELUM ADA IMPLEMENTASI VIEW NYA 
                break;
            case Command::CETAK_AKTA : 
                string code; 
                cout << "Masukkan kode petak: ";
                cin >> code; 
                // MANGGIL FUNGSI VIEW LAGI 
                break;
            case Command::CETAK_PROPERTI :
                cout << "=== Properti Milik: " << currentPlayer->getName() << " ===" << endl;
                // MANGGIL FUNGSI YANG ADA DI VIEW LAGI
                break;
            case Command::GADAI: 
                
        }

    }

}