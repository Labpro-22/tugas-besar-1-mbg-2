#pragma once
#include "PropertyTile.hpp"

using namespace std;

class RailroadTile : public PropertyTile
{
public:
    RailroadTile(int idx, string code, string name, int price, int morgageValue);
    LandResult land(GameContext &G) override;
};
