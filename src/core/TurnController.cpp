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

    switch (result.getType()) {
        case LandEventType::OFFERBUYPROPERTY:{
            display.renderBuyStreet(*context, dynamic_cast<StreetTile*>(currentTile));
            input.getStringInput();
            string choice = input.getLastStringInput();
            if (choice == "Y" || choice == "y") {
                try {
                    eco.purchaseProperty(*player, dynamic_cast<PropertyTile*>(currentTile));
                    display.renderBoughtStreet(*context, dynamic_cast<StreetTile*>(currentTile));
                } catch (const InsufficientFundsException& ex) {
                    throw AuctionTriggerException();
                }   
            } else {
                throw AuctionTriggerException();
            }
            break;
        }

        case LandEventType::GIVEPROPERTY:{
            PropertyTile* propTile = dynamic_cast<PropertyTile*>(currentTile);
            eco.acquirePropertyFree(*player, propTile);
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
                Player* owner = dynamic_cast<PropertyTile*>(currentTile)->getOwner();
                eco.payRent(*player, *owner, dynamic_cast<PropertyTile*>(currentTile), dice.getTotal());
                display.renderCurrentBalancePayed(*context, rentAmount);
            } catch (const InsufficientFundsException& ex) {
                display.renderCantPay(*context, ex.getRequired());
                throw BankruptcyException("Player cannot afford to pay rent.", ex.getRequired(), player->getBalance(), dynamic_cast<PropertyTile*>(currentTile)->getOwner(), currentTile);
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
                        int balanceBeforeTax = player->getBalance();
                        display.renderPayTax(*context, choice);
                        eco.processIncomeTax(context, *player, choice);
                        int balanceAfterTax = player->getBalance();
                        int taxPaid = balanceBeforeTax - balanceAfterTax;
                        display.renderCurrentBalancePayed(*context, taxPaid);
                    } catch (const InsufficientFundsException& ex) {
                        display.renderCantPayTax(*context, ex.getRequired());
                        throw BankruptcyException("Player cannot afford to pay income tax.", ex.getRequired(), player->getBalance(), nullptr, taxTile);
                    }
                } else {
                    try {
                        eco.processLuxuryTax(context, *player);
                        display.renderCurrentBalancePayed(*context, context->getPbm());
                    } catch (const InsufficientFundsException& ex) {
                        display.renderCantPayTax(*context, ex.getRequired());
                        display.renderCurrentBalance(*context);
                        throw BankruptcyException("Player cannot afford to pay luxury tax.", ex.getRequired(), player->getBalance(), nullptr, taxTile);
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
            display.renderCardTile(*context, drawnCard);
            if (drawnCard != nullptr) {
                eff.execute(*drawnCard, *player, *context);
                context->getChanceDeck().returnAndShuffle(drawnCard);
            }
            break;
        }

        case LandEventType::DRAWCOMMUNITYCHEST: {
            try {
                ActionCard* drawnCard = context->getCommunityChestDeck().draw();
                display.renderCardTile(*context, drawnCard);
                if (drawnCard != nullptr) {
                    eff.execute(*drawnCard, *player, *context);
                    context->getCommunityChestDeck().returnAndShuffle(drawnCard);
                }
            } catch (const InsufficientFundsException& ex) {
                display.renderCantPay(*context, ex.getRequired());
                throw BankruptcyException("Player cannot afford to pay due to a Community Chest card.", ex.getRequired(), player->getBalance(), nullptr, context->getBoard().getTile(player->getPosition()));
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

void TurnController::handleBuildHouse(GameContext* context, Player* player, EconomyController& eco, InputHandler& input, DisplayView& display) {
    map<string, vector<StreetTile*>> groupColor = player->getColorOwnedStreetTile();
    map<string, vector<StreetTile*>> buildableStreet = eco.buildableStreet(groupColor, context, *player);

    if (buildableStreet.empty()) {
        display.renderInfo("You don't have any properties eligible for building houses!");
        return;
    }

    display.renderBuildStart(*context, buildableStreet);

    input.getIntInput();
    int choice = input.getIntValue1();
    
    if (choice == 0){
        display.renderBuildCancel(*context);
        return;
    }

    if (choice < 0 || choice > buildableStreet.size()) {
        display.renderBuildInvalid(*context);
        return;
    }

    auto itMap = buildableStreet.begin();
    advance(itMap, choice - 1);

    string color = itMap->first;
    vector<StreetTile*> allBuildableStreet = itMap->second;

    display.renderBuildMid(*context, allBuildableStreet);
    
    input.getIntInput();
    int propertyChoice = input.getIntValue1();

    if (propertyChoice == 0) {
        display.renderBuildCancel(*context);
        return;
    }

    if (propertyChoice < 0 || propertyChoice > allBuildableStreet.size()) {
        display.renderBuildInvalid(*context);
        return;
    }

    StreetTile* chosenTile = allBuildableStreet[propertyChoice - 1];
    
    try
    {
        if (eco.canBuildOnTile(context, chosenTile)) {
            eco.buildHouse(context, *player, chosenTile);
        } else if (eco.canUpgradeToHotel(context, color)) {
            eco.upgradeToHotel(context, *player, chosenTile);
        } else {
            display.renderInfo("This property cannot be built on anymore.");
            return;
        }
        display.renderbuildHouses(*context, chosenTile, allBuildableStreet);
    }
    catch(const InsufficientFundsException& ex)
    {
        display.renderInfo("You don't have enough funds to build on this property. Required: " + to_string(ex.getRequired()) + ", Your Balance: " + to_string(ex.getCurrentBalance()));
    }
}