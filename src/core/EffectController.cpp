#include "EffectController.hpp"
#include "EconomyController.hpp"
#include "BankruptcyController.hpp"
#include "ActionCard.hpp"
#include "SkillCard.hpp"
#include "MoveCard.hpp"       
#include "DiscountCard.hpp"
#include "RailroadTile.hpp"
#include "InputHandler.hpp"
#include "DisplayView.hpp"
#include "GameException.hpp"
#include <iostream>

void EffectController::handleFestival(GameContext* gameContext, DisplayView* display, InputHandler* inputHandler, GameLogger& logger){
    
    Player& currentPlayer = gameContext->getCurrentPlayer();
    vector<StreetTile*> streetTile;

    for (auto i : currentPlayer.getOwnedProperties()){
        if (auto* tile = dynamic_cast<StreetTile*>(i)){
            streetTile.push_back(tile);
        }
    }

    if (streetTile.empty()){
        display->renderInfo("You don't have any street tile to apply festival on!\n");
        return;
    }

    display->renderFestivalTile(*gameContext, streetTile);
    string choice;
    while(true){
        inputHandler->getStringInput();
        choice = inputHandler->getLastStringInput();
        if (codeInOwned(choice, streetTile)){
            break;
        }
        else {
            if (codeInBoard( choice, gameContext)) {
                display->InputUnvalidFestivalProperty(*gameContext, streetTile, true);
            }else{
                display->InputUnvalidFestivalProperty(*gameContext, streetTile, false);
            }
        }
    }
    StreetTile* choosed = dynamic_cast<StreetTile*>(gameContext->getBoard().getTileByCode( choice ));
    if (choosed->isFestivalActive()){
        choosed->playerReenterFestival();
        logger.addLog(gameContext->getCurrentTurn(), currentPlayer.getName(), "FESTIVAL", "Re-entered festival on " + choosed->getName());
    }else{
        choosed->applyFestival();
        logger.addLog(gameContext->getCurrentTurn(), currentPlayer.getName(), "FESTIVAL", "Activated festival on " + choosed->getName());
    }
}

void EffectController::decrementDurations(GameContext* context){
    Tile* tile = context->getBoard().getTile(context->getCurrentPlayer().getPosition());
    StreetTile* s = dynamic_cast<StreetTile*>(tile);
    if (s != nullptr) {
        s->decreaseFestivalTurn();
    }
}

void EffectController::execute(ActionCard& card, Player& currentPlayer, GameContext& ctx, BankruptcyController& bank, InputHandler& inputHandler, DisplayView& display, EconomyController& eco, GameLogger& logger) {
    switch (card.getActionType()) {
        case ActionCardType::MOVE_TO_STATION: {
            int currentPos = currentPlayer.getPosition();
    
            int stationPos = ctx.getBoard().findNearestStation(currentPos);
            
            currentPlayer.setPosition(stationPos);
            logger.addLog(ctx.getCurrentTurn(), currentPlayer.getName(), "MOVE_TO_STATION_CARD", "Moved to nearest station");
            break;
        }
        case ActionCardType::MOVE_BACKWARD: {
            if (currentPlayer.hasShield()) {
                break;
            }

            int currentPos = currentPlayer.getPosition();
            int newPos = ctx.getBoard().calculateTargetPosition(currentPos, -3);
            currentPlayer.setPosition(newPos);
            logger.addLog(ctx.getCurrentTurn(), currentPlayer.getName(), "MOVE_BACKWARD_CARD", "Moved backward 3 steps");
            break;
        }
        case ActionCardType::MOVE_TO_JAIL: {
            if (currentPlayer.hasShield()) {
                break;
            }

            Tile* jailTile = ctx.getBoard().getTileByCode("PEN");
            
            if (jailTile != nullptr) {
                int jailPos = jailTile->getIdx();
                currentPlayer.setPosition(jailPos);
                currentPlayer.setStatus(PlayerStatus::JAILED);
                currentPlayer.setJailTurns(0); 
                currentPlayer.setDoubleCount(0);
                logger.addLog(ctx.getCurrentTurn(), currentPlayer.getName(), "MOVE_TO_JAIL_CARD", "Moved to jail");
            }
            break;
        }
        case ActionCardType::BIRTHDAY: {
            for (Player& p : ctx.getPlayers()) {
                if (&p != &currentPlayer && p.getStatus() != PlayerStatus::BANKRUPT) {
                    try {
                        p -= 100;
                        currentPlayer += 100;
                        logger.addLog(ctx.getCurrentTurn(), currentPlayer.getName(), "BIRTHDAY_CARD", "Collected M100 from " + p.getName());
                    } catch (const InsufficientFundsException& ex) {
                        bank.liquidateAssets(ctx, p, &currentPlayer, ex.getRequired(), display, eco, inputHandler, ctx.getBoard().getTile(p.getPosition()), logger);
                    }
                }
            }
            break;
        }
        case ActionCardType::DOCTOR_FEE: {

            if (currentPlayer.hasShield()) {
                break;
            }

            try {
                currentPlayer -= 700;
                logger.addLog(ctx.getCurrentTurn(), currentPlayer.getName(), "DOCTOR_FEE_CARD", "Paid M700 doctor fee");
            } catch (const InsufficientFundsException& ex) {
                throw BankruptcyException("You don't have enough money to pay the doctor's fee.", ex.getRequired(), currentPlayer.getBalance(), nullptr,  ctx.getBoard().getTile(currentPlayer.getPosition()));
            }
            break;
        }
        case ActionCardType::NYALEG: {
            if (currentPlayer.hasShield()) {
                break;
            }

            for (Player& p : ctx.getPlayers()) {
                if (&p != &currentPlayer && p.getStatus() != PlayerStatus::BANKRUPT) {
                    try {
                        currentPlayer -= 200;
                        p += 200;
                        logger.addLog(ctx.getCurrentTurn(), currentPlayer.getName(), "NYALEG_CARD", "Paid M200 to " + p.getName());
                    }
                    catch (const InsufficientFundsException& ex) {
                        bank.liquidateAssets(ctx, currentPlayer, &p, ex.getRequired(), display, eco, inputHandler, ctx.getBoard().getTile(currentPlayer.getPosition()), logger);
                    }
                }
            }
            break;
        }
    }
}

void EffectController::execute(SkillCard& card, Player& currentPlayer, GameContext& ctx, InputHandler& input, DisplayView& display, GameLogger& logger) {
    switch (card.getSkillType()) {
        case SkillCardType::MOVE: {
            MoveCard& mCard = dynamic_cast<MoveCard&>(card);
            
            int steps = mCard.getSteps();
            int currentPos = currentPlayer.getPosition();
            
            int newPos = ctx.getBoard().calculateTargetPosition(currentPos, steps);
            currentPlayer.setPosition(newPos);
            logger.addLog(ctx.getCurrentTurn(), currentPlayer.getName(), "USE_SKILL_CARD", "Used Move card to move " + to_string(steps) + " steps");
            break;
        }
        case SkillCardType::DISCOUNT: {
            DiscountCard& dCard = dynamic_cast<DiscountCard&>(card);
            
            currentPlayer.applyDiscount(dCard.getDiscountPercentage());
            logger.addLog(ctx.getCurrentTurn(), currentPlayer.getName(), "USE_SKILL_CARD", "Used Discount card (" + to_string(dCard.getDiscountPercentage()) + "% off)");
            break;
        }
        case SkillCardType::SHIELD: {

            currentPlayer.applyShield(); 
            logger.addLog(ctx.getCurrentTurn(), currentPlayer.getName(), "USE_SKILL_CARD", "Used Shield card");
            break;
        }
        case SkillCardType::TELEPORT: {
            Tile* targetTile = nullptr;
            string targetCode;
            while (true) {
                display.renderInfo("Enter the code of the tile you want to teleport to: ");
                input.getStringInput();
                targetCode = input.getLastStringInput();
                
                targetTile = ctx.getBoard().getTileByCode(targetCode);
                
                if (targetTile != nullptr) {
                    break; 
                } 
                else {
                    display.renderInfo("[ERROR] Tile code '" + targetCode + "' not found!\n");
                    display.renderInfo("Please enter a valid tile code: ");
                }
            }

            int targetIdx = targetTile->getIdx();
            currentPlayer.setPosition(targetIdx);
            display.renderInfo("Teleporting to " + targetTile->getName() + "...");
            logger.addLog(ctx.getCurrentTurn(), currentPlayer.getName(), "USE_SKILL_CARD", "Used Teleport card to " + targetCode);
            break;
        }
        case SkillCardType::LASSO: {
            int currentPos = currentPlayer.getPosition();
            display.renderInfo("List of opponent players that can be pulled:\n");
            display.renderPlayer(ctx);

            display.renderInfo("Enter the name of the opponent player you want to pull: ");

            Player* targetPlayer = nullptr;

            while (true) {
                input.getStringInput();
                string targetName = input.getLastStringInput();

                targetPlayer = nullptr;

                for (Player& p : ctx.getPlayers()) {
                    if (p.getName() == targetName) {
                        targetPlayer = &p;
                        break;
                    }
                }

                if (targetPlayer == nullptr) {
                    display.renderInfo("[ERROR] Player with the name '" + targetName + "' not found!\n");
                    display.renderInfo("Enter the name correctly (case-sensitive): ");
                    continue;
                }

                if (targetPlayer == &currentPlayer) {
                    display.renderInfo("[ERROR] You cannot use Lasso on yourself!\n");
                    display.renderInfo("Enter an OPPONENT'S name: ");
                    continue;
                }

                if (targetPlayer->getStatus() == PlayerStatus::BANKRUPT) {
                    display.renderInfo("[ERROR] " + targetName + " is already bankrupt and out of the game!\n");
                    display.renderInfo("Enter the name of an active player: ");
                    continue;
                }

                break;
            }

            targetPlayer->setPosition(currentPos);    
            logger.addLog(ctx.getCurrentTurn(), currentPlayer.getName(), "USE_SKILL_CARD", "Used Lasso card to pull " + targetPlayer->getName() + " to position " + to_string(currentPos));
            break;
        }
        case SkillCardType::DEMOLITION: {
            Tile* targetTile = nullptr;
            StreetTile* targetStreet = nullptr;

            while (true) {
                display.renderInfo("Enter the code of the opponent's street tile you want to demolish: ");
                input.getStringInput();
                string targetCode = input.getLastStringInput();

                targetTile = ctx.getBoard().getTileByCode(targetCode);

                if (targetTile == nullptr) {
                    display.renderInfo("[ERROR] Tile code '" + targetCode + "' not found!\n");
                    display.renderInfo("Enter the opponent's street tile code: ");
                    continue;
                }

                targetStreet = dynamic_cast<StreetTile*>(targetTile);
                if (targetStreet == nullptr) {
                    display.renderInfo("[ERROR] Tile " + targetTile->getName() + " is not a destructible tile!\n");
                    display.renderInfo("Enter the opponent's street tile code: ");
                    continue; 
                }

                Player* owner = targetStreet->getOwner();
                
                if (owner == nullptr) {
                    display.renderInfo("[ERROR] This property is an empty land / not owned by anyone!\n");
                    display.renderInfo("Enter the opponent's street tile code: ");
                    continue;
                }

                if (owner == &currentPlayer) {
                    display.renderInfo("[ERROR] You cannot destroy your own property!\n");
                    display.renderInfo("Enter an OPPONENT'S street tile code: ");
                    continue;
                }

                if (owner->getStatus() == PlayerStatus::BANKRUPT) {
                    display.renderInfo("[ERROR] The owner of this property is already bankrupt!\n");
                    display.renderInfo("Enter the street tile code of an active opponent: ");
                    continue;
                }

                break; 
            }

            targetStreet->demolishBuilding();
            display.renderInfo("Demolishing buildings on " + targetStreet->getName() + "...");

            logger.addLog(ctx.getCurrentTurn(), currentPlayer.getName(), "USE_SKILL_CARD", "Used Demolition card on " + targetStreet->getName());
            break;
        }
        case SkillCardType::JAILFREE: {

            currentPlayer.setStatus(PlayerStatus::ACTIVE);
            currentPlayer.setJailTurns(0);
            logger.addLog(ctx.getCurrentTurn(), currentPlayer.getName(), "USE_SKILL_CARD", "Used Jail Free card");
        }
    }
}

bool EffectController::codeInOwned(string code, vector<StreetTile*> streetTile){

    for (auto i : streetTile){
        if (code == i->getCode()){
            return true;
        }
    }
    return false;
}


bool EffectController::codeInBoard(string code, GameContext* g){
    for (auto i : g->getBoard().getPropertyTile()){
        if (code == i->getCode()){
            return true;
        }
    }
    return false;
}