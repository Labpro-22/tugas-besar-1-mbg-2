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

void TurnController::resolveTileLanding(GameContext* context, Player* player, EconomyController& eco, EffectController& eff, AuctionController& auc, BankruptcyController& bank, Dice& dice, SaveLoader& sl, InputHandler& input, GameLogger& logger) {
    
    Tile* currentTile = context->getBoard().getTile(player->getPosition());
    cout << "\n>> [" << player->getName() << "] mendarat di petak: " << currentTile->getName() << " <<" << endl;

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

        case LandEventType::DRAWCHANCE: {
            ActionCard* drawnCard = context->getChanceDeck().draw();
            
            if (drawnCard != nullptr) {
                eff.execute(*drawnCard, *player, *context);
                context->getChanceDeck().returnAndShuffle(drawnCard);
            }
            break;
        }

        case LandEventType::DRAWCOMMUNITYCHEST: {
            ActionCard* drawnCard = context->getCommunityChestDeck().draw();
            
            if (drawnCard != nullptr) {
                eff.execute(*drawnCard, *player, *context);
                context->getCommunityChestDeck().returnAndShuffle(drawnCard);
            }
            break;
        }

        case LandEventType::GOTOJAIL:
            cout << "Anda mendarat di petak Go To Jail, Anda dipenjara!" << endl;
            player->setStatus(PlayerStatus::JAILED);
            player->setPosition(context->getBoard().getTileByCode("PEN")->getIdx()); // indeks penjara
            player->setJailTurns(0);
            break;

        case LandEventType::BANKRUPTCYCHECK:
            // bank.declareBankruptcy(*player);
            break;

        case LandEventType::DONOTHING:
        case LandEventType::NONE:
        default:
            break;
    }
}

void TurnController::handleDiceRollMovement(GameContext* context, EconomyController& eco, EffectController& eff, AuctionController& auc, BankruptcyController& bank, Dice& dice, SaveLoader& sl, InputHandler& input, GameLogger& logger) {
    
    Player* currentPlayer = &context->getCurrentPlayer(); 
    
    int diceTotal = dice.getTotal(); 
    cout << "Hasil Dadu: " << dice.getDice1() << " + " << dice.getDice2() << " = " << diceTotal << endl;
    cout << "Memajukan Bidak " << currentPlayer->getName() << " sebanyak " << diceTotal << " petak..." << endl;
    
    int oldPos = currentPlayer->getPosition();
    int newPos = context->getBoard().calculateTargetPosition(oldPos, diceTotal);
    currentPlayer->setPosition(newPos);
    
    if (currentPlayer->getPosition() < oldPos) {
        *currentPlayer += context->getGoSalary();
        cout << "Melewati GO! Mendapat gaji M" << context->getGoSalary() << endl;
    }

    resolveTileLanding(context, currentPlayer, eco, eff, auc, bank, dice, sl, input, logger);
}