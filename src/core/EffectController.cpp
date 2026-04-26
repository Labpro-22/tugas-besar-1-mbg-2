#include "EffectController.hpp"
#include "ActionCard.hpp"
#include "SkillCard.hpp"
#include "MoveCard.hpp"       
#include "DiscountCard.hpp"
#include "RailroadTile.hpp"
#include "InputHandler.hpp"
#include "DisplayView.hpp"
#include <iostream>

void EffectController::handleFestival(GameContext* gameContext, DisplayView* display, InputHandler* inputHandler){
    
    Player& currentPlayer = gameContext->getCurrentPlayer();
    vector<StreetTile*> streetTile;
    for (auto i : currentPlayer.getOwnedProperties()){
        if (auto* tile = dynamic_cast<StreetTile*>(i)){
            streetTile.push_back(tile);
        }
    }
    // display->renderFestivalTile(&gameContext);
    inputHandler->getStringInput();
    string choice = inputHandler->getLastStringInput();
    while(true){
        if (codeInOwned(choice, streetTile)){
            break;
        }
        else {
            if (codeInBoard( choice, gameContext)) {
                // display->InputUnvalidFestivalProperty(gameContext, true);
            }else{
                // display->InputUnvalidFestivalProperty(gameContext, false);
            }
        }
    }
    StreetTile* choosed = dynamic_cast<StreetTile*>(gameContext->getBoard().getTileByCode( choice ));
    if (choosed->isFestivalActive()){
        choosed->playerReenterFestival();
    }else{
        choosed->applyFestival();
    }
}

void EffectController::decrementDurations(GameContext* context){
    Tile* tile = context->getBoard().getTile(context->getCurrentPlayer().getPosition());
    StreetTile* s = dynamic_cast<StreetTile*>(tile);
    s->decreaseFestivalTurn();
    // minimal kosong dulu
}
void EffectController::execute(ActionCard& card, Player& currentPlayer, GameContext& ctx) {
    switch (card.getActionType()) {
        case ActionCardType::MOVE_TO_STATION: {
            int currentPos = currentPlayer.getPosition();
    
            int stationPos = ctx.getBoard().findNearestStation(currentPos);
            
            currentPlayer.setPosition(stationPos);
            break;
        }
        case ActionCardType::MOVE_BACKWARD: {
            if (currentPlayer.hasShield()) {
                break;
            }

            int currentPos = currentPlayer.getPosition();
            int newPos = ctx.getBoard().calculateTargetPosition(currentPos, -3);
            currentPlayer.setPosition(newPos);
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
            }
            break;
        }
        case ActionCardType::BIRTHDAY: {
            for (Player& p : ctx.getPlayers()) {
                if (&p != &currentPlayer && p.getStatus() != PlayerStatus::BANKRUPT) {
                    p -= 100;
                    currentPlayer += 100;
                }
            }
            break;
        }
        case ActionCardType::DOCTOR_FEE: {
            if (currentPlayer.hasShield()) {
                break;
            }

            currentPlayer -= 700;
            break;
        }
        case ActionCardType::NYALEG: {
            if (currentPlayer.hasShield()) {
                break;
            }

            for (Player& p : ctx.getPlayers()) {
                if (&p != &currentPlayer && p.getStatus() != PlayerStatus::BANKRUPT) {
                    currentPlayer -= 200;
                    p += 200;
                }
            }
            break;
        }
    }
}

void EffectController::execute(SkillCard& card, Player& currentPlayer, GameContext& ctx, InputHandler& input, DisplayView& display) {
    switch (card.getSkillType()) {
        case SkillCardType::MOVE: {
            MoveCard& mCard = dynamic_cast<MoveCard&>(card);
            
            int steps = mCard.getSteps();
            int currentPos = currentPlayer.getPosition();
            
            int newPos = ctx.getBoard().calculateTargetPosition(currentPos, steps);
            currentPlayer.setPosition(newPos);
            break;
        }
        case SkillCardType::DISCOUNT: {
            DiscountCard& dCard = dynamic_cast<DiscountCard&>(card);
            
            currentPlayer.applyDiscount(dCard.getDiscountPercentage());
            break;
        }
        case SkillCardType::SHIELD: {
            currentPlayer.applyShield(); 
            break;
        }
        case SkillCardType::TELEPORT: {
            Tile* targetTile = nullptr;
            while (true) {
                input.getStringInput();
                string targetCode = input.getLastStringInput();
                
                targetTile = ctx.getBoard().getTileByCode(targetCode);
                
                if (targetTile != nullptr) {
                    break; 
                } 
                else {
                    cout << "[ERROR] Tile code '" << targetCode << "' not found!\n";
                    cout << "Please enter a valid tile code: ";
                }
            }

            int targetIdx = targetTile->getIdx();
            currentPlayer.setPosition(targetIdx);
            
            break;
        }
        case SkillCardType::LASSO: {
            int currentPos = currentPlayer.getPosition();
            cout << "List of opponent players that can be pulled:\n";
            display.renderPlayer(ctx);

            cout << "Enter the name of the opponent player you want to pull: ";
            
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
                    cout << "[ERROR] Player with the name '" << targetName << "' not found!\n";
                    cout << "Enter the name correctly (case-sensitive): ";
                    continue;
                }

                if (targetPlayer == &currentPlayer) {
                    cout << "[ERROR] You cannot use Lasso on yourself!\n";
                    cout << "Enter an OPPONENT'S name: ";
                    continue;
                }

                if (targetPlayer->getStatus() == PlayerStatus::BANKRUPT) {
                    cout << "[ERROR] " << targetName << " is already bankrupt and out of the game!\n";
                    cout << "Enter the name of an active player: ";
                    continue;
                }

                break;
            }

            targetPlayer->setPosition(currentPos);    
            
            break;
        }
        case SkillCardType::DEMOLITION: {
            Tile* targetTile = nullptr;
            StreetTile* targetStreet = nullptr;

            while (true) {
                input.getStringInput();
                string targetCode = input.getLastStringInput();

                targetTile = ctx.getBoard().getTileByCode(targetCode);

                if (targetTile == nullptr) {
                    cout << "[ERROR] Tile code '" << targetCode << "' not found!\n";
                    cout << "Enter the opponent's street tile code: ";
                    continue;
                }

                targetStreet = dynamic_cast<StreetTile*>(targetTile);
                if (targetStreet == nullptr) {
                    cout << "[ERROR] Tile " << targetTile->getName() << " is not a destructible tile!\n";
                    cout << "Enter the opponent's street tile code: ";
                    continue; 
                }

                Player* owner = targetStreet->getOwner();
                
                if (owner == nullptr) {
                    cout << "[ERROR] This property is an empty land / not owned by anyone!\n";
                    cout << "Enter the opponent's street tile code: ";
                    continue;
                }

                if (owner == &currentPlayer) {
                    cout << "[ERROR] You cannot destroy your own property!\n";
                    cout << "Enter an OPPONENT'S street tile code: ";
                    continue;
                }

                if (owner->getStatus() == PlayerStatus::BANKRUPT) {
                    cout << "[ERROR] The owner of this property is already bankrupt!\n";
                    cout << "Enter the street tile code of an active opponent: ";
                    continue;
                }

                break; 
            }

            targetStreet->demolishBuilding();
            break;
        }
        case SkillCardType::JAILFREE: {
            currentPlayer.setStatus(PlayerStatus::ACTIVE);
            currentPlayer.setJailTurns(0);
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