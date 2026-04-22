#pragma once
#include "PropertyTile.hpp"

using namespace std;

class UtilityTile : public PropertyTile
{ 
public:
    UtilityTile(int idx, string code, string name, int price, 
        int morgageValue, string color);
    LandResult land(GameContext &G) override;
};
