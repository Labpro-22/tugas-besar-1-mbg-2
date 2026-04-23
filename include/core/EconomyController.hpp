#pragma once
#include "StreetTile.hpp"
#include "Dice.hpp"
#include "Player.hpp"
#include <map>

using namespace std;
class EconomyController
{
public:
    int calculateRent(PropertyTile p, Dice d);
    void processTax(Player& p);
    bool buildProperty(Player& p, StreetTile& tile);
    
    void addRailroadRent(int totalOwned, int rent);
    void addUtilityMultiplier(int totalOwned, int multiplier);
    void setGoSalary(int amount);
    void setPph(int amount);
    void setPbm(int amount);
    void setJailFine(int amount);
};
