#pragma once
#include "Player.hpp"

class BankruptcyController
{
private:
    Player* debtor;
    Player* creditor;
    int debtAmount;
public:
    bool checkSolvency(Player& p, int amount);
    void liquidateAssets(Player& p); 
    void declareBankruptcy(Player& p, Player& creditor);
};
