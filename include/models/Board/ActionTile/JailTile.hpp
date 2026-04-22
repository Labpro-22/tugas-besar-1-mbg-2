#pragma once
#include <string>
#include "ActionTile.hpp"

using namespace std;

class JailTile : public ActionTile
{
public:
    JailTile(int idx, std::string code, std::string name);
    LandResult land(GameContext &G) override;
};