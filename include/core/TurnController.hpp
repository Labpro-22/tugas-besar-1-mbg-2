#pragma once
#include <list>
#include "Player.hpp"
#include "GameContext.hpp"
#include "EffectController.hpp"
#include "EconomyController.hpp"
#include "AuctionController.hpp"
#include "BankruptcyController.hpp"
#include "SaveLoader.hpp"
#include "Dice.hpp"
#include "InputHandler.hpp"
#include "GameLogger.hpp"

using namespace std;

class TurnController
{
public:
    bool checkWinCondition();
    void TurnController::distributeSkillCards(GameContext& ctx, InputHandler& input);
    void resolveTileLanding(GameContext* context, Player* player, EconomyController& eco, EffectController& eff, AuctionController& auc, BankruptcyController& bank, Dice& dice, SaveLoader& sl, InputHandler& input, GameLogger& logger);
    void handleDiceRollMovement(GameContext* context, EconomyController& eco, EffectController& eff, AuctionController& auc, BankruptcyController& bank, Dice& dice, SaveLoader& sl, InputHandler& input, GameLogger& logger);
};
