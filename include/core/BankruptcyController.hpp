#pragma once
#include "Player.hpp"

class BankruptcyController
{
public:
    bool checkSolvency(Player& p, int amount);
    void liquidateAssets(Player& p); 
    void declareBankruptcy(Player& p, Player& creditor);
};
