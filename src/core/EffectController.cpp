#include "EffectController.hpp"
#include "ActionCard.hpp"
#include "SkillCard.hpp"
#include "MoveCard.hpp"       
#include "DiscountCard.hpp"
#include "RailroadTile.hpp"
#include "InputHandler.hpp"
#include "DisplayView.hpp"
#include <iostream>

void EffectController::execute(ActionCard& card, Player& currentPlayer, GameContext& ctx) {
    switch (card.getActionType()) {
        case ActionCardType::MOVE_TO_STATION: {
            int currentPos = currentPlayer.getPosition();
            int totalTiles = ctx.getBoard().getTotalTile();
            int searchPos = (currentPos + 1) % totalTiles;

            while (true) {
                Tile* tileToCheck = ctx.getBoard().getTile(searchPos);
                RailroadTile* rrTile = dynamic_cast<RailroadTile*>(tileToCheck);
                
                if (rrTile != nullptr) { 
                    break;
                }
                searchPos = (searchPos + 1) % totalTiles;
            }

            currentPlayer.setPosition(searchPos);
            break;
        }
        case ActionCardType::MOVE_BACKWARD: {
            int currentPos = currentPlayer.getPosition();
            int totalTiles = ctx.getBoard().getTotalTile();
            int newPos = (currentPos - 3 + totalTiles) % totalTiles; 
            currentPlayer.setPosition(newPos);
            break;
        }
        case ActionCardType::MOVE_TO_JAIL: {
            Tile* jailTile = ctx.getBoard().getTileByCode("PEN");
            
            if (jailTile != nullptr) {
                int jailPos = jailTile->getIdx();
                currentPlayer.setPosition(jailPos);
                currentPlayer.setStatus(PlayerStatus::JAILED); 
            }
            break;
        }
        case ActionCardType::BIRTHDAY: {
            for (Player& p : ctx.getPlayers()) {
                if (&p != &currentPlayer && p.getStatus() != PlayerStatus::BANKRUPT) {
                    p.reduceBalance(100);
                    currentPlayer.addBalance(100);
                }
            }
            break;
        }
        case ActionCardType::DOCTOR_FEE: {
            currentPlayer.reduceBalance(700);
            break;
        }
        case ActionCardType::NYALEG: {
            for (Player& p : ctx.getPlayers()) {
                if (&p != &currentPlayer && p.getStatus() != PlayerStatus::BANKRUPT) {
                    currentPlayer.reduceBalance(200);
                    p.addBalance(200);
                }
            }
            break;
        }
    }
}

void EffectController::execute(SkillCard& card, Player& currentPlayer, GameContext& ctx, InputHandler& input) {
    switch (card.getSkillType()) {
        case SkillCardType::MOVE: {
            MoveCard& mCard = dynamic_cast<MoveCard&>(card);
            
            int steps = mCard.getSteps();
            int currentPos = currentPlayer.getPosition();
            int totalTiles = ctx.getBoard().getTotalTile();
            
            int newPos = (currentPos + steps) % totalTiles;
            currentPlayer.setPosition(newPos);
            break;
        }
        case SkillCardType::DISCOUNT: {
            DiscountCard& dCard = dynamic_cast<DiscountCard&>(card);
            
            currentPlayer.applyDiscount(dCard.getDiscountPercentage(), dCard.getDuration());
            break;
        }
        case SkillCardType::SHIELD: {
            currentPlayer.applyShield(1); 
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
            DisplayView::renderPlayer();

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
            PropertyTile* targetProperty = nullptr;

            while (true) {
                input.getStringInput();
                string targetCode = input.getLastStringInput();

                targetTile = ctx.getBoard().getTileByCode(targetCode);

                if (targetTile == nullptr) {
                    cout << "[ERROR] Tile code '" << targetCode << "' not found!\n";
                    cout << "Enter the opponent's property tile code: ";
                    continue;
                }

                targetProperty = dynamic_cast<PropertyTile*>(targetTile);
                if (targetProperty == nullptr) {
                    cout << "[ERROR] Tile " << targetTile->getName() << " is not a destructible property!\n";
                    cout << "Enter the opponent's property tile code: ";
                    continue; 
                }

                Player* owner = targetProperty->getOwner();
                
                if (owner == nullptr) {
                    cout << "[ERROR] This property is an empty land / not owned by anyone!\n";
                    cout << "Enter the opponent's property tile code: ";
                    continue;
                }

                if (owner == &currentPlayer) {
                    cout << "[ERROR] You cannot destroy your own property!\n";
                    cout << "Enter an OPPONENT'S property tile code: ";
                    continue;
                }

                if (owner->getStatus() == PlayerStatus::BANKRUPT) {
                    cout << "[ERROR] The owner of this property is already bankrupt!\n";
                    cout << "Enter the property tile code of an active opponent: ";
                    continue;
                }

                break; 
            }

            // Eksekusi penghancuran bangunan
            
            break;
        }
    }
}