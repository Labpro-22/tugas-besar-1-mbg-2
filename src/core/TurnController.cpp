#include "TurnController.hpp"
#include "GameException.hpp"
#include <iostream>

using namespace std;

void TurnController::distributeSkillCards(GameContext& ctx, InputHandler& input, DisplayView& display) {
    for (Player& p : ctx.getPlayers()) {
        if (p.getStatus() == PlayerStatus::BANKRUPT) continue; 

        SkillCard* newCard = ctx.getSkillDeck().draw();
        
        display.renderReceiveSkillCard(p, newCard);
        p.addCardToHand(newCard);         

        if (!p.canHoldMoreSkillCards()) {
            display.renderMaxSkillCard(p);

            int maxChoice = p.getSkillCardCount();
            int choice = -1;
            while (true) {
                input.getIntInput();
                choice = input.getIntValue1();
                display.renderDiscardSkillCard(p, choice);

                if (choice >= 1 && choice <= maxChoice) {
                    break;
                }
            }

            int vectorIndex = choice - 1;
            SkillCard* discardedCard = p.dropSkillCard(vectorIndex); 
            ctx.getSkillDeck().discard(discardedCard);
        }
    }
}

void TurnController::resolveTileLanding(GameContext* context, Player* player, EconomyController& eco, EffectController& eff, AuctionController& auc, BankruptcyController& bank, Dice& dice, SaveLoader& sl, InputHandler& input, GameLogger& logger, DisplayView& display) {
    
    Tile* currentTile = context->getBoard().getTile(player->getPosition());
    display.renderTile(*context);
    LandResult result = currentTile->land(*context);

    switch (result.type) {
        case LandEventType::OFFERBUYPROPERTY:{
            display.renderBuyStreet(*context, dynamic_cast<StreetTile*>(currentTile));
            input.getStringInput();
            string choice = input.getLastStringInput();
            if (choice == "Y" || choice == "y") {
                try {
                    eco.purchaseProperty(*player, dynamic_cast<PropertyTile*>(currentTile));
                } catch (const InsufficientFundsException& ex) {
                    throw AuctionTriggerException();
                }
            } else {
                throw AuctionTriggerException();
            }
            break;
        }

        case LandEventType::GIVEPROPERTY:{
            player->addProperty(dynamic_cast<PropertyTile*>(currentTile));
            if (currentTile->getType() == "Railroad"){
                display.renderGetRailroad(*context, dynamic_cast<RailroadTile*>(currentTile));
            } else if (currentTile->getType() == "Utility"){
                display.renderGetUtility(*context, dynamic_cast<UtilityTile*>(currentTile));
            } 
            break;
        }


        case LandEventType::PAYRENT:
            try {
                display.renderRent(*context, dynamic_cast<PropertyTile*>(currentTile));
                int rentAmount = eco.calculateRent(context, dynamic_cast<PropertyTile*>(currentTile), dice.getTotal());
                *player -= rentAmount;
                Player* owner = dynamic_cast<PropertyTile*>(currentTile)->getOwner();
                if (owner != nullptr) {
                    *owner += rentAmount;
                }
            } catch (const InsufficientFundsException& ex) {
                display.renderCantPay(*context, ex.getRequired());
                throw BankruptcyException("Player cannot afford to pay rent.", ex.getRequired(), player->getBalance());
            }
            // eco.handlePayRent(result, context, bank, diceTotal);
            break;

        case LandEventType::PAYTAX:
            display.renderTax(*context, dynamic_cast<TaxTile*>(currentTile));
            if (auto* taxTile = dynamic_cast<TaxTile*>(currentTile)) {
                if (taxTile->getIsPPH()) {
                    input.getIntInput();
                    int choice = input.getIntValue1();
                    try {
                        display.renderPayTax(*context, choice);
                        eco.processIncomeTax(context, *player, choice);
                        display.renderCurrentBalancePayed(*context, choice);
                    } catch (const InsufficientFundsException& ex) {
                        display.renderCantPayTax(*context, ex.getRequired());
                        throw BankruptcyException("Player cannot afford to pay income tax.", ex.getRequired(), player->getBalance());
                    }
                } else {
                    try {
                        eco.processLuxuryTax(context, *player);
                        display.renderCurrentBalancePayed(*context, context->getPbm());
                    } catch (const InsufficientFundsException& ex) {
                        display.renderCantPayTax(*context, ex.getRequired());
                        display.renderCurrentBalance(*context);
                        throw BankruptcyException("Player cannot afford to pay luxury tax.", ex.getRequired(), player->getBalance());
                    }
                }
            }
            break;

        case LandEventType::DOFESTIVAL:{
           eff.handleFestival(context, &display, &input );
            break;
        }

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
            display.renderInfo("[" + player->getName() + "] landed on 'Go To Jail' tile!");
            player->setStatus(PlayerStatus::JAILED);
            player->setPosition(context->getBoard().getTileByCode("PEN")->getIdx()); // indeks penjara
            player->setJailTurns(0);
            break;

        case LandEventType::DONOTHING:
        case LandEventType::NONE:
        default:
            break;
    }
}

void TurnController::handleDiceRollMovement(GameContext* context, EconomyController& eco, EffectController& eff, AuctionController& auc, BankruptcyController& bank, Dice& dice, SaveLoader& sl, InputHandler& input, GameLogger& logger, DisplayView& display) {
    
    Player* currentPlayer = &context->getCurrentPlayer(); 
    
    int diceTotal = dice.getTotal(); 
    
    int oldPos = currentPlayer->getPosition();
    int newPos = context->getBoard().calculateTargetPosition(oldPos, diceTotal);
    currentPlayer->setPosition(newPos);
    
    if (currentPlayer->getPosition() < oldPos) {
        *currentPlayer += context->getGoSalary();
        display.renderInfo("You passed GO! Collected " + to_string(context->getGoSalary()) + " coins.");
    }

    resolveTileLanding(context, currentPlayer, eco, eff, auc, bank, dice, sl, input, logger, display);
}