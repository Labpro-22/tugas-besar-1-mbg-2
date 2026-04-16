#pragma once
#include "PropertyTile.hpp"

class Railroad : public PropertyTile
{
public:
    Railroad(int idx, string code, string name, int price, int morgageValue, vector<int> rentPrices);
    PropertyStatus land(Player &p) override;

    PropertyStatus land(Player &p) override;
};
