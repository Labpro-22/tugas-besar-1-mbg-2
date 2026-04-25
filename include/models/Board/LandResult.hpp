#pragma once
#include <string>

using namespace std;

// Forward declarations to avoid circular includes
class Tile;
class Player;

enum class LandEventType
{
    NONE,
    DOFESTIVAL,
    GIVEPROPERTY,
    OFFERBUYPROPERTY,
    PAYRENT,
    PAYTAX,
    GOTOJAIL,
    DRAWCHANCE,
    DRAWCOMMUNITYCHEST,
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
};
