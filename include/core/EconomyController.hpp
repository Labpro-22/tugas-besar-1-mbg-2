#pragma once
#include "StreetTile.hpp"
#include "Dice.hpp"
#include "Player.hpp"


class EconomyController

{
private:
    int goSalary;
    int pph;
    int pbm;
public:
    int calculateRent(PropertyTile p, Dice d);
    void processTax(Player& p);
    bool buildProperty(Player& p, StreetTile& tile);
};
