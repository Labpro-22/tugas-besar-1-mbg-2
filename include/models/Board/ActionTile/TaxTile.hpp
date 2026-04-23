#pragma once
#include "ActionTile.hpp"

using namespace std;

class TaxTile : public ActionTile
{
private:
    bool isPPH; // true for PPh, false for PBM
public:
    TaxTile(int idx, string code, string name, bool isPPH, string color = "DEFAULT");
    LandResult land(GameContext &G) override;
};
