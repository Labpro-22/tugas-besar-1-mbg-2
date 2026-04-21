#pragma once
#include <string>
#include "Tile.hpp"
#include "Player.hpp"
using namespace std;

enum class LandEventType
{
    NONE,
    OFFERBUYPROPERTY,
    PAYRENT,
    PAYTAX,
    GAINMONEY,
    GOTOJAIL,
    DRAWCARD,
    STARTAUCTION,
    BANKRUPTCYCHECK,
    DONOTHING
};

struct LandResult
{
    LandEventType type = LandEventType::NONE;
    Tile* sourceTile = nullptr;
    Tile* targetTile = nullptr;
    Player* sourcePlayer = nullptr;
    Player* targetPlayer = nullptr;
    int amount = 0;
    bool requiresChoice = false;
    std::string message = "";
};
