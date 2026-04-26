#pragma once
#include <string>
#include "ActionTile.hpp"
#include "GameContext.hpp"

using namespace std;

class JailTile : public ActionTile
{
public:
    JailTile(int idx, std::string code, std::string name, string color = "DEFAULT");
    LandResult land(GameContext &G) override;
};