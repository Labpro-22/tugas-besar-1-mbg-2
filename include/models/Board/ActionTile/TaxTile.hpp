#pragma once
#include "ActionTile.hpp"

class TaxTile : public ActionTile
{
private:
    bool isPPH; // true for PPh, false for PBM
public:
    TaxTile(int idx, string code, string name, bool isPPH);
    void land(Player &p) override;
};
