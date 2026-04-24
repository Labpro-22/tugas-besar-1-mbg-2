#include "TurnController.hpp"
#include <iostream>

using namespace std;

void TurnController::distributeSkillCards(GameContext& ctx, InputHandler& input) {
    for (Player& p : ctx.getPlayers()) {
        if (p.getStatus() == PlayerStatus::BANKRUPT) continue; 

        SkillCard* newCard = ctx.getSkillDeck().draw();
        
        std::cout << "\n>> " << p.getName() << " draws a card: [" << newCard->getName() << "]\n";

        if (p.canHoldMoreSkillCards()) {
            p.addCardToHand(newCard); 
            std::cout << "Card added to " << p.getName() << "'s hand.\n";
        } 
        else {
            std::cout << "[ATTENTION] " << p.getName() << ", your hand is full!\n";
            std::cout << "You MUST discard one of your existing cards to make room for [" << newCard->getName() << "].\n";
            
            int displayIdx = 1;
            
            for (SkillCard* c : p.getSkillCard()) {
                std::cout << "[" << displayIdx << "] " << c->getName() << " - " << c->getDescription() << "\n";
                displayIdx++;
            }

            int maxChoice = p.getSkillCardCount();
            std::cout << "Select a card number to discard (1-" << maxChoice << "): ";
            
            int choice = -1;
            while (true) {
                input.getIntInput();
                choice = input.getIntValue1();

                if (choice >= 1 && choice <= maxChoice) {
                    break;
                }
                std::cout << "[ERROR] Invalid choice! Select a card number (1-" << maxChoice << "): ";
            }

            int vectorIndex = choice - 1;
            SkillCard* discardedCard = p.dropSkillCard(vectorIndex); 
            ctx.getSkillDeck().discard(discardedCard);

            p.addCardToHand(newCard);
            
            std::cout << "Card [" << discardedCard->getName() << "] discarded to the Discard Pile.\n";
            std::cout << "New card [" << newCard->getName() << "] added to hand.\n";
        }
    }
}

void TurnController::executeAction(GameContext* context, EconomyController& eco, EffectController& eff, AuctionController& auc, BankruptcyController& bank, Dice& dice, SaveLoader& sl, InputHandler& input, GameLogger& logger) {
    Player* currentPlayer = &context->getCurrentPlayer(); 
    
    int diceTotal = dice.getTotal(); // dice udah di roll 
    cout << "Hasil Dadu: " << dice.getDice1() << " + " << dice.getDice2() << " = " << diceTotal << endl;
    cout << "Memajukan Bidak " << currentPlayer->getName() << " sebanyak " << diceTotal << " petak..." << endl;
    
    int oldPos = currentPlayer->getPosition();
    int newPos = context->getBoard().calculateTargetPosition(oldPos, diceTotal);
    currentPlayer->setPosition(newPos);
    
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

        case LandEventType::DRAWCHANCE:
            ActionCard* drawnCard = context->getChanceDeck().draw();
            
            if (drawnCard != nullptr) {
                eff.execute(*drawnCard, *currentPlayer, *context);
                context->getChanceDeck().returnAndShuffle(drawnCard);
            }
            break;

        case LandEventType::DRAWCOMMUNITYCHEST:
            ActionCard* drawnCard = context->getCommunityChestDeck().draw();
            
            if (drawnCard != nullptr) {
                eff.execute(*drawnCard, *currentPlayer, *context);
                context->getCommunityChestDeck().returnAndShuffle(drawnCard);
            }
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