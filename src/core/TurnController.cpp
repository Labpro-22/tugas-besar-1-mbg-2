#include "TurnController.hpp"
#include <iostream>

using namespace std;

void TurnController::executeAction(GameContext* context, EconomyController& eco, EffectController& eff, AuctionController& auc, BankruptcyController& bank, Dice& dice, SaveLoader& sl, InputHandler& input, GameLogger& logger) {
    Player* currentPlayer = &context->getCurrentPlayer(); 
    
    int diceTotal = dice.getTotal(); // dice udah di roll 
    cout << "Hasil Dadu: " << dice.getDice1() << " + " << dice.getDice2() << " = " << diceTotal << endl;
    cout << "Memajukan Bidak " << currentPlayer->getName() << " sebanyak " << diceTotal << " petak..." << endl;
    
    int oldPos = currentPlayer->getPosition();
    currentPlayer->move(diceTotal);
    
    if (currentPlayer->getPosition() < oldPos) {
        *currentPlayer += context->getGoSalary();
        cout << "Melewati GO! Mendapat gaji M" << context->getGoSalary() << endl;
    }

    Tile* currentTile = context->getBoard().getTile(currentPlayer->getPosition());
    cout << "Bidak mendarat di " << currentTile->getName() << "." << endl;
    
    // Kalau ada log harusnya ditambahin disini

    LandResult result = currentTile->land(*context);

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
            // cout << "Anda dapat meng-upgrade properti ini." << endl; ini gatau implementasinya gimana wkwk
            // eco.handleOfferUpgradeProperty(result, context, bank);
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
            cout << "Anda mendarat di petak Go To Jail, Anda dipenjara!" << endl;
            currentPlayer->setStatus(PlayerStatus::JAILED);
            currentPlayer->setPosition(10); // indeks penjara harusnya
            currentPlayer->setJailTurns(0);
            break;

        case LandEventType::BANKRUPTCYCHECK:
            bank.declareBankruptcy(*currentPlayer);
            break;

        case LandEventType::DONOTHING:
        case LandEventType::NONE:
        default:
            break;
    }

    eff.decrementDurations();

    if (dice.isDouble() && currentPlayer->getStatus() == PlayerStatus::ACTIVE && currentPlayer->getJailTurns() == 0) {
        cout << currentPlayer->getName() << " mendapat dadu double! Silakan main lagi." << endl;
    } else {
        context->nextPlayer();
    }
}