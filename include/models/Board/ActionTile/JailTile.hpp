#pragma once
#include "ActionTile.hpp"

class JailTile : public ActionTile
{
public:
    JailTile(int idx, string code, string name);
    void land(Player &p) override;
};

