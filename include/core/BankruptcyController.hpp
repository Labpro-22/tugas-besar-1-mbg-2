#pragma once
#include "Player.hpp"
#include "PropertyTile.hpp"
#include "DisplayView.hpp"
#include "StreetTile.hpp"
#include "EconomyController.hpp"
#include "GameContext.hpp"
#include "InputHandler.hpp"
#include "Liquidation.hpp"
#include "GameLogger.hpp"
#include <numeric>
#include <algorithm>
#include <vector>
#include <map>


class BankruptcyController
{
public:
    vector<LiquidationTile> bestLiquidationAsset(Player& player, int amount);
    
    bool checkSolvency(Player& p, int amount);
    void transferProperties(Player& debitor, Player& creditor);
    
    void applyAction(GameContext& ctx, Player& player, LiquidationOption& opt, EconomyController& eco, GameLogger& logger);
    vector<LiquidationOption> generateOptions(Player& player, const vector<LiquidationOption>& cart, int& initBalance);
    void liquidateAssets(GameContext& ctx, Player& debitor, Player* creditor, int amount, DisplayView& view, EconomyController& eco, InputHandler& input, Tile* bankruptTile, GameLogger& logger); 
    void declareBankruptcy(GameContext& ctx, Player& player, Player& creditor, int amount, DisplayView& view, PropertyTile* tile, GameLogger& logger);
    void declareBankruptcy(GameContext& ctx, Player& player, int amount, DisplayView& view, EconomyController& eco, TaxTile* tile, InputHandler& input, GameLogger& logger); // Ini kalau bangkrut nya ke bank
    void declareBankruptcy(GameContext& ctx, Player& player, int amount, DisplayView& view, EconomyController& eco, InputHandler& input, GameLogger& logger);
    void handleInsufficientFunds(GameContext& ctx, Player& debitor, Player* creditor, int amount, EconomyController& eco, DisplayView& view, InputHandler& input, GameLogger& logger);
    int calculateTotal(Player& player, const vector<LiquidationTile>& best);
    vector<string> bestLiquidation(Player& player, const vector<LiquidationTile>& best);
};
