#pragma once
#include "Player.hpp"
#include "PropertyTile.hpp"
#include "StreetTile.hpp"
#include <numeric>

class BankruptcyController
{
public:
    int calculatePropertySell(PropertyTile* property);
    int calculateMaxLiquidation(Player& player);
    void transferProperties(Player& debitor, Player& creditor);

    bool checkSolvency(Player& p, int amount);
    void liquidateAssets(Player& p); 
    void declareBankruptcy(Player& p, Player& creditor);
    void declareBankruptcy(Player& p); // Ini kalau bangkrut nya ke bank
};
