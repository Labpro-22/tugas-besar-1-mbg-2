#pragma once
#include "ActionTile.hpp"

using namespace std;

class GoToJailTile : public ActionTile
{
public:
    GoToJailTile(int idx, string code, string name, string color = "DEFAULT");
    LandResult land(GameContext &G) override;
};

