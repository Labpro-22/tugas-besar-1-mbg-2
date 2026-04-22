#pragma once
#include "PropertyTile.hpp"

using namespace std;

class UtilityTile : public PropertyTile
{ 
public:
    UtilityTile(int idx, string code, string name, int price, 
        int morgageValue, string color);
    LandResult land(GameContext &G) override;
    string getTypeLabel() const override;
    vector<string> getAktaDetailLines(const GameContext& G) const override;
    vector<string> getRentDetailLines(GameContext& G) const override;
    int calculateRent(GameContext& G) const override;
    string getPropertyDisplayInfo() const override;

};
