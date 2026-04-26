#pragma once
#include "GameView.hpp"
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
    void distributeSkillCards(GameContext& ctx, InputHandler& input, DisplayView& display, bool isGUIMode = false, GameView* guiView = nullptr);
    void resolveTileLanding(GameContext* context, Player* player, EconomyController& eco, EffectController& eff, AuctionController& auc, BankruptcyController& bank, Dice& dice, SaveLoader& sl, InputHandler& input, GameLogger& logger, DisplayView& display, bool isGUIMode = false, GameView* guiView = nullptr);
    void handleDiceRollMovement(GameContext* context, EconomyController& eco, EffectController& eff, AuctionController& auc, BankruptcyController& bank, Dice& dice, SaveLoader& sl, InputHandler& input, GameLogger& logger, DisplayView& display, bool isGUIMode = false, GameView* guiView = nullptr);
    void handleBuildHouse(GameContext* context, Player* player, EconomyController& eco, InputHandler& input, DisplayView& display, bool isGUIMode = false, GameView* guiView = nullptr);
};
