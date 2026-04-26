#include "TurnController.hpp"
#include "GameException.hpp"
#include <iostream>

using namespace std;

void TurnController::distributeSkillCards(GameContext& ctx, InputHandler& input, DisplayView& display, bool isGUIMode, GameView* guiView) {
    for (Player& p : ctx.getPlayers()) {
        if (p.getStatus() == PlayerStatus::BANKRUPT) continue; 

        SkillCard* newCard = ctx.getSkillDeck().draw();
        
        display.renderReceiveSkillCard(p, newCard);
        p.addCardToHand(newCard);         

        if (!p.canHoldMoreSkillCards()) {
            display.renderMaxSkillCard(p);

            int maxChoice = p.getSkillCardCount();
            int choice = -1;
            
            if (isGUIMode && guiView != nullptr) {
                choice = guiView->getSkillCardChoice(p.getSkillCard(), true);
                if (choice == -1) choice = 1; 
                else choice += 1; 
            } else {
                while (true) {
                    input.getIntInput();
                    choice = input.getIntValue1();
                    display.renderDiscardSkillCard(p, choice);

                    if (choice >= 1 && choice <= maxChoice) {
                        break;
                    }
                }
            }

            int vectorIndex = choice - 1;
            SkillCard* discardedCard = p.dropSkillCard(vectorIndex); 
            ctx.getSkillDeck().discard(discardedCard);
        }
    }
}

void TurnController::resolveTileLanding(GameContext* context, Player* player, EconomyController& eco, EffectController& eff, AuctionController& auc, BankruptcyController& bank, Dice& dice, SaveLoader& sl, InputHandler& input, GameLogger& logger, DisplayView& display, bool isGUIMode, GameView* guiView) {
    
    Tile* currentTile = context->getBoard().getTile(player->getPosition());
    display.renderTile(*context);
    LandResult result = currentTile->land(*player);

    if (isGUIMode && guiView != nullptr &&
        result.getType() != LandEventType::OFFERBUYPROPERTY &&
        result.getType() != LandEventType::PAYTAX &&
        result.getType() != LandEventType::DOFESTIVAL &&
        result.getType() != LandEventType::DRAWCHANCE &&
        result.getType() != LandEventType::DRAWCOMMUNITYCHEST &&
        result.getType() != LandEventType::GOTOJAIL) {
        guiView->triggerPopup("LANDING", currentTile);
        guiView->getPopupResponse();
    }

    switch (result.getType()) {
        case LandEventType::OFFERBUYPROPERTY:{
            display.renderBuyStreet(*context, dynamic_cast<StreetTile*>(currentTile));

            std::string choice = "n";
            if (isGUIMode && guiView != nullptr) {
                guiView->triggerPopup("PROPERTY", currentTile); 
                int res = guiView->getPopupResponse();          
                
                if (res == 1) choice = "y";      
                else choice = "n";               
            } else {
                input.getStringInput();
                choice = input.getLastStringInput();
            }

            if (choice == "Y" || choice == "y") {
                try {
                    PropertyTile* property = dynamic_cast<PropertyTile*>(currentTile);
                    eco.purchaseProperty(*player, property);
                    display.renderBoughtStreet(*context, dynamic_cast<StreetTile*>(currentTile));
                    logger.addLog(context->getCurrentTurn(), player->getName(), "BUY_PROPERTY", property->getName() + " bought for M" + to_string(property->getPrice()));
                } catch (const InsufficientFundsException& ex) {
                    if (isGUIMode && guiView != nullptr) {
                        guiView->triggerPopup("AUCTION", currentTile);
                        guiView->getPopupResponse();
                    }
                    throw AuctionTriggerException();
                }
            } else {
                if (isGUIMode && guiView != nullptr) {
                    guiView->triggerPopup("AUCTION", currentTile);
                    guiView->getPopupResponse();
                }
                throw AuctionTriggerException();
            }
            break;
        }

        case LandEventType::GIVEPROPERTY:{
            PropertyTile* propTile = dynamic_cast<PropertyTile*>(currentTile);
            eco.acquirePropertyFree(*player, propTile);

            logger.addLog(context->getCurrentTurn(), player->getName(), "ACQUIRE_PROPERTY", propTile->getName() + " acquired for free");
            if (RailroadTile* railroadTile = dynamic_cast<RailroadTile*>(currentTile)) {
                display.renderGetRailroad(*context, railroadTile);
            } else if (UtilityTile* utilityTile = dynamic_cast<UtilityTile*>(currentTile)) {
                display.renderGetUtility(*context, utilityTile);
            } else {
                display.renderInfo("You received " + currentTile->getName() + " for free.");
            break;
        }


        case LandEventType::PAYRENT:
            try {
                display.renderRent(*context, dynamic_cast<PropertyTile*>(currentTile));
                int rentAmount = eco.calculateRent(context, dynamic_cast<PropertyTile*>(currentTile), dice.getTotal());

                if(player->hasShield()){
                    display.renderShieldPay(*context, player, rentAmount);
                    break;
                }

                Player* owner = dynamic_cast<PropertyTile*>(currentTile)->getOwner();
                eco.payRent(*player, *owner, dynamic_cast<PropertyTile*>(currentTile), dice.getTotal());

                logger.addLog(context->getCurrentTurn(), player->getName(), "PAY_RENT", "Paid M" + to_string(rentAmount) + " to " + owner->getName() + " for " + currentTile->getName());
            } catch (const InsufficientFundsException& ex) {
                display.renderCantPay(*context, ex.getRequired());
                throw BankruptcyException("Player cannot afford to pay rent.", ex.getRequired(), player->getBalance(), dynamic_cast<PropertyTile*>(currentTile)->getOwner(), currentTile);
            }
            break;

        case LandEventType::PAYTAX:
            display.renderTax(*context, dynamic_cast<TaxTile*>(currentTile));
            if(player->hasShield()){
                display.renderInfo("Your shield protected you from paying the tax!");
                break;
            }
            if (auto* taxTile = dynamic_cast<TaxTile*>(currentTile)) {
                if (taxTile->getIsPPH()) {
                    int choice = 1;

                    if (isGUIMode && guiView != nullptr) {
                        guiView->triggerPopup("TAX_PPH", currentTile);
                        int res = guiView->getPopupResponse();
                        choice = (res == 2) ? 2 : 1;
                    } else {
                        input.getIntInput();
                        choice = input.getIntValue1();
                    }

                    try {
                        int balanceBeforeTax = player->getBalance();
                        display.renderPayTax(*context, choice);
                        eco.processIncomeTax(context, *player, choice);
                        int balanceAfterTax = player->getBalance();
                        int taxPaid = balanceBeforeTax - balanceAfterTax;
                        display.renderCurrentBalancePayed(*context, taxPaid);
                        logger.addLog(context->getCurrentTurn(), player->getName(), "PAY_TAX", "Income tax paid M" + to_string(taxPaid));
                    } catch (const InsufficientFundsException& ex) {
                        display.renderCantPayTax(*context, ex.getRequired());
                        throw BankruptcyException("Player cannot afford to pay income tax.", ex.getRequired(), player->getBalance(), nullptr, taxTile);
                    }
                } else {
                    try {
                        if (isGUIMode && guiView != nullptr) {
                            guiView->triggerPopup("TAX_PPBM", currentTile);
                            guiView->getPopupResponse();
                        }
                        eco.processLuxuryTax(context, *player);
                        display.renderCurrentBalancePayed(*context, context->getPbm());
                        logger.addLog(context->getCurrentTurn(), player->getName(), "PAY_TAX", "Luxury tax paid M" + to_string(context->getPbm()));
                    } catch (const InsufficientFundsException& ex) {
                        display.renderCantPayTax(*context, ex.getRequired());
                        display.renderCurrentBalance(*context);
                        throw BankruptcyException("Player cannot afford to pay luxury tax.", ex.getRequired(), player->getBalance(), nullptr, taxTile);
                    }
                }
            }
            break;

        case LandEventType::DOFESTIVAL:{
            if (isGUIMode && guiView != nullptr) {
                guiView->triggerPopup("FESTIVAL", currentTile);
                int popupRes = guiView->getPopupResponse();

                if (popupRes == 1) {
                    int selectedTileIndex = guiView->getLastPopupTileIndex();
                    if (selectedTileIndex < 0) {
                        display.renderWarning("Festival target is invalid. No festival effect applied.");
                        break;
                    }

                    Tile* chosenTile = context->getBoard().getTile(selectedTileIndex);
                    StreetTile* selectedStreet = dynamic_cast<StreetTile*>(chosenTile);

                    if (selectedStreet != nullptr) {
                        if (selectedStreet->isFestivalActive()) {
                            selectedStreet->playerReenterFestival();
                        } else {
                            selectedStreet->applyFestival();
                        }
                        display.renderFestivalResult(*context, selectedStreet);
                    } else {
                        display.renderWarning("Festival target is invalid. No festival effect applied.");
                    }
                } else {
                    display.renderInfo("Festival action canceled.");
                }
            } else {
                eff.handleFestival(context, &display, &input, logger);
            }
            
            break;
        }

        case LandEventType::DRAWCHANCE: {
            ActionCard* drawnCard = context->getChanceDeck().draw();
            display.renderCardTile(*context, drawnCard);
            if (drawnCard != nullptr) {
                if (isGUIMode && guiView != nullptr) {
                    guiView->triggerPopup("CHANCE", currentTile, drawnCard->getName() + ": " + drawnCard->getDescription());
                    guiView->getPopupResponse();
                }
                eff.execute(*drawnCard, *player, *context, bank, input, display, eco);
                logger.addLog(context->getCurrentTurn(), player->getName(), "DRAW_CHANCE_CARD", drawnCard->getName());
                
                int posisiAwal = player->getPosition(); 
                
                eff.execute(*drawnCard, *player, *context, bank, input, display, eco, logger);
                context->getChanceDeck().returnAndShuffle(drawnCard);

                if (player->getPosition() != posisiAwal) {
                    display.renderInfo("\n[CARD EFFECT] " + player->getName() + " landed on a new tile!");
                    resolveTileLanding(context, player, eco, eff, auc, bank, dice, sl, input, logger, display);
                }
            }
            break;
        }

        case LandEventType::DRAWCOMMUNITYCHEST: {
            try {
                ActionCard* drawnCard = context->getCommunityChestDeck().draw();
                display.renderCardTile(*context, drawnCard);
                if (drawnCard != nullptr) {
                    if (isGUIMode && guiView != nullptr) {
                        guiView->triggerPopup("COMMUNITY", currentTile, drawnCard->getName() + ": " + drawnCard->getDescription());
                        guiView->getPopupResponse();
                    }
                    eff.execute(*drawnCard, *player, *context, bank, input, display, eco);
                    logger.addLog(context->getCurrentTurn(), player->getName(), "DRAW_COMMUNITY_CHEST_CARD", drawnCard->getName());
                    
                    int posisiAwal = player->getPosition(); 

                    eff.execute(*drawnCard, *player, *context, bank, input, display, eco, logger);
                    context->getCommunityChestDeck().returnAndShuffle(drawnCard);

                    if (player->getPosition() != posisiAwal) {
                        display.renderInfo("\n[CARD EFFECT] " + player->getName() + " landed on a new tile!");
                        resolveTileLanding(context, player, eco, eff, auc, bank, dice, sl, input, logger, display);
                    }
                }
            } catch (const InsufficientFundsException& ex) {
                display.renderCantPay(*context, ex.getRequired());
                throw BankruptcyException("Player cannot afford to pay due to a Community Chest card.", ex.getRequired(), player->getBalance(), nullptr, context->getBoard().getTile(player->getPosition()));
            }
            break;
        }

        case LandEventType::GOTOJAIL:
            display.renderInfo("[" + player->getName() + "] landed on 'Go To Jail' tile!");
            if (isGUIMode && guiView != nullptr) {
                guiView->triggerPopup("JAIL", currentTile);
                guiView->getPopupResponse();
            }
            player->setStatus(PlayerStatus::JAILED);
            player->setPosition(context->getBoard().getIndexOfTile(context->getBoard().getTileByCode("PEN"))); // indeks penjara
            player->setJailTurns(0);
            break;

        case LandEventType::DONOTHING:
        case LandEventType::NONE:
        default:
            break;
    }
}

void TurnController::handleDiceRollMovement(GameContext* context, EconomyController& eco, EffectController& eff, AuctionController& auc, BankruptcyController& bank, Dice& dice, SaveLoader& sl, InputHandler& input, GameLogger& logger, DisplayView& display, bool isGUIMode, GameView* guiView) {
    
    Player* currentPlayer = &context->getCurrentPlayer(); 
    
    int diceTotal = dice.getTotal(); 
    logger.addLog(context->getCurrentTurn(), currentPlayer->getName(), "ROLL_DICE", "Rolled " + to_string(diceTotal));
    
    int oldPos = currentPlayer->getPosition();
    int newPos = context->getBoard().calculateTargetPosition(oldPos, diceTotal);
    currentPlayer->setPosition(newPos);
    logger.addLog(context->getCurrentTurn(), currentPlayer->getName(), "MOVE", "Moved from " + to_string(oldPos) + " to " + to_string(newPos));
    
    if (currentPlayer->getPosition() < oldPos) {
        *currentPlayer += context->getGoSalary();
        display.renderInfo("You passed GO! Collected " + to_string(context->getGoSalary()) + " coins.");
        logger.addLog(context->getCurrentTurn(), currentPlayer->getName(), "PASS_GO", "Collected M" + to_string(context->getGoSalary()));
    }

    if (isGUIMode && guiView != nullptr) {
        guiView->updateBoardState(*context);
    }

    resolveTileLanding(context, currentPlayer, eco, eff, auc, bank, dice, sl, input, logger, display, isGUIMode, guiView);
}

void TurnController::handleBuildHouse(GameContext* context, Player* player, EconomyController& eco, InputHandler& input, GameLogger& logger, DisplayView& display) {
    map<string, vector<StreetTile*>> groupColor = player->getColorOwnedStreetTile();
    map<string, vector<StreetTile*>> buildableStreet = eco.buildableStreet(groupColor, context, *player);

    if (buildableStreet.empty()) {
        display.renderInfo("You don't have any properties eligible for building houses!");
        if (isGUIMode && guiView != nullptr) {
            guiView->showInfoPopup("Build", "You don't have any properties eligible for building!\nRequirement: own all tiles in a color group.");
        }
        return;
    }

    display.renderBuildStart(*context, buildableStreet);

    int choice = -1;
    if (isGUIMode && guiView != nullptr) {
        std::vector<std::string> groupOptions;
        for (const auto& entry : buildableStreet) {
            std::string info = "[" + entry.first + "]";
            for (StreetTile* t : entry.second) {
                if (!t->getHasHotel()) {
                    info += " " + t->getName() + "(" + std::to_string(t->getHouseCount()) + "H)";
                }
            }
            groupOptions.push_back(info);
        }
        int idx = guiView->getIntChoiceFromList("Select Color Group to build in:", groupOptions);
        if (idx == -1) {
            display.renderBuildCancel(*context);
            return;
        }
        choice = idx + 1;
    } else {
        input.getIntInput();
        choice = input.getIntValue1();
    }
    
    if (choice == 0) {
        display.renderBuildCancel(*context);
        return;
    }

    if (choice < 0 || choice > (int)buildableStreet.size()) {
        display.renderBuildInvalid(*context);
        return;
    }

    auto itMap = buildableStreet.begin();
    advance(itMap, choice - 1);

    string color = itMap->first;
    vector<StreetTile*> allBuildableStreet = itMap->second;

    display.renderBuildMid(*context, allBuildableStreet);
    
    int propertyChoice = -1;
    if (isGUIMode && guiView != nullptr) {
        std::vector<std::string> tileOptions;
        for (StreetTile* t : allBuildableStreet) {
            std::string info = t->getName() + " (" + t->getCode() + ")";
            if (t->getHasHotel()) info += " [Hotel - can't build]";
            else info += " [" + std::to_string(t->getHouseCount()) + " Houses] Cost: M" + std::to_string(t->getHouseCost());
            tileOptions.push_back(info);
        }
        int idx = guiView->getIntChoiceFromList("Select property to build in [" + color + "]:", tileOptions);
        if (idx == -1) {
            display.renderBuildCancel(*context);
            return;
        }
        propertyChoice = idx + 1;
    } else {
        input.getIntInput();
        propertyChoice = input.getIntValue1();
    }

    if (propertyChoice == 0) {
        display.renderBuildCancel(*context);
        return;
    }

    if (propertyChoice < 0 || propertyChoice > (int)allBuildableStreet.size()) {
        display.renderBuildInvalid(*context);
        return;
    }

    StreetTile* chosenTile = allBuildableStreet[propertyChoice - 1];
    
    try {
        if (eco.canBuildOnTile(context, chosenTile)) {
            eco.buildHouse(context, *player, chosenTile);
            logger.addLog(context->getCurrentTurn(), player->getName(), "BUILD_HOUSE", "Built house on " + chosenTile->getName());
        } else if (eco.canUpgradeToHotel(context, color)) {
            eco.upgradeToHotel(context, *player, chosenTile);
            logger.addLog(context->getCurrentTurn(), player->getName(), "UPGRADE_HOTEL", "Upgraded " + chosenTile->getName() + " to hotel");
        } else {
            display.renderInfo("This property cannot be built on anymore.");
            return;
        }
        display.renderbuildHouses(*context, chosenTile, allBuildableStreet);
    }
    catch(const InsufficientFundsException& ex) {
        display.renderInfo("You don't have enough funds to build on this property. Required: " + to_string(ex.getRequired()) + ", Your Balance: " + to_string(ex.getCurrentBalance()));
    }
}