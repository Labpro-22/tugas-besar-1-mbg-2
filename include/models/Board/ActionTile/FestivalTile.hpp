#pragma once
#include "ActionTile.hpp"
#include "GameContext.hpp"

using namespace std;

class FestivalTile : public ActionTile
{    
public:
    FestivalTile(int idx, string code, string name, string color);
    LandResult land(GameContext &G) override;
};


