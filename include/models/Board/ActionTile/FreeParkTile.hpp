#pragma once
#include "ActionTile.hpp"
#include "GameContext.hpp"

using namespace std;

class FreeParkTile : public ActionTile
{
public:
    FreeParkTile(int idx, string code, string name, string color = "DEFAULT");
    LandResult land(Player &p) override;
};