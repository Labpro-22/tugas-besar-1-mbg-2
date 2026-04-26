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
#include "DisplayView.hpp"

using namespace std;

class TurnController
{
public:
    bool checkWinCondition();
    void distributeSkillCards(GameContext& ctx, InputHandler& input, DisplayView& display);
    void resolveTileLanding(GameContext* context, Player* player, EconomyController& eco, EffectController& eff, AuctionController& auc, BankruptcyController& bank, Dice& dice, SaveLoader& sl, InputHandler& input, GameLogger& logger, DisplayView& display);
    void handleDiceRollMovement(GameContext* context, EconomyController& eco, EffectController& eff, AuctionController& auc, BankruptcyController& bank, Dice& dice, SaveLoader& sl, InputHandler& input, GameLogger& logger, DisplayView& display);
};
