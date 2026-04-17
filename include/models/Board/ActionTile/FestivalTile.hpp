#pragma once
#include "ActionTile.hpp"

class FestivalTile : public ActionTile
{    
public:
    FestivalTile(int idx, string code, string name);
    void land(Player &p) override;
};


