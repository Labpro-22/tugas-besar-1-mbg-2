
#pragma once
#include "ActionTile.hpp"

class StartTile : public ActionTile
{
public:
    StartTile(int idx, string code, string name);
    void land(Player &p) override;
};