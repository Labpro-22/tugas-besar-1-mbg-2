#pragma once
#include "ActionTile.hpp"

using namespace std;

class FreeParkTile : public ActionTile
{
public:
    FreeParkTile(int idx, string code, string name);
    LandResult land(GameContext &G) override;
};