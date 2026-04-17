#pragma once
#include "ActionTile.hpp"

class GoToJailTile : public ActionTile
{
public:
    GoToJailTile(int idx, string code, string name);
    void land(Player &p) override;
};

