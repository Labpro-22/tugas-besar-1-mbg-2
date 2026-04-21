#pragma once
#include "ActionTile.hpp"

using namespace std;

class FestivalTile : public ActionTile
{    
public:
    FestivalTile(int idx, string code, string name);
    LandResult land(GameContext &G) override;
};


