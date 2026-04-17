#pragma once
#include "ActionTile.hpp"

class FreeParkTile : public ActionTile
{
public:
    FreeParkTile(int idx, string code, string name);
    void land(Player &p) override;
};