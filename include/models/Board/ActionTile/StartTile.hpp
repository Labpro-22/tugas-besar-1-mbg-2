
#pragma once
#include "ActionTile.hpp"
using namespace std;

using namespace std;

class StartTile : public ActionTile
{
public:
    StartTile(int idx, string code, string name, string color = "DEFAULT");
    LandResult land(GameContext &G) override;
};