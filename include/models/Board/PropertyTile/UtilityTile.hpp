#pragma once
#include "PropertyTile.hpp"

using namespace std;

class UtilityTile : public PropertyTile
{ 
public:
    UtilityTile(int idx, string code, string name, int price, 
        int morgageValue);
    LandResult land(GameContext &G) override;
};
