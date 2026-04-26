#pragma once
#include "Player.hpp"
#include "PropertyTile.hpp"
#include "DisplayView.hpp"
#include "StreetTile.hpp"
#include "EconomyController.hpp"
#include "GameContext.hpp"
#include "InputHandler.hpp"
#include "Liquidation.hpp"
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
    
    void applyAction(GameContext& ctx,Player& player, LiquidationOption& opt, EconomyController& eco);
    vector<LiquidationOption> generateOptions(Player& player, const vector<LiquidationOption>& cart, int& initBalance);
    void liquidateAssets(GameContext& ctx, Player& debitor, Player* creditor, int amount, DisplayView& view, EconomyController& eco, InputHandler& input, Tile* bankruptTile); 
    void declareBankruptcy(GameContext& ctx, Player& player, Player& creditor, int amount, DisplayView& view, PropertyTile* tile);
    void declareBankruptcy(GameContext& ctx, Player& player, int amount, DisplayView& view, EconomyController& eco, TaxTile* tile, InputHandler& input); // Ini kalau bangkrut nya ke bank
    void declareBankruptcy(GameContext& ctx, Player& player, int amount, DisplayView& view, EconomyController& eco, InputHandler& input);
    void handleInsufficientFunds(GameContext& ctx, Player& debitor, Player* creditor, int amount, EconomyController& eco, DisplayView& view, InputHandler& input);
};
