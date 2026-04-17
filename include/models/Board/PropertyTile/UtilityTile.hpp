#pragma once
#include "PropertyTile.hpp"

class UtilityTile : public PropertyTile
{ 
public:
    UtilityTile(int idx, string code, string name, int price, 
        int morgageValue, vector<int> rentPrices);
    PropertyStatus land(Player &p) override;
};
