
#pragma once
#include "ActionTile.hpp"
using namespace std;

using namespace std;

class StartTile : public ActionTile
{
public:
    StartTile(int idx, string code, string name);
    LandResult land(GameContext &G) override;
};