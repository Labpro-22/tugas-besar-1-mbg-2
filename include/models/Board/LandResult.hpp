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

class LandResult
{
    private:
        LandEventType type = LandEventType::NONE;
        Tile* sourceTile = nullptr;
        Tile* targetTile = nullptr;
        Player* sourcePlayer = nullptr;
        Player* targetPlayer = nullptr;
        int amount = 0;
        bool requiresChoice = false;
    public:
        LandResult() = default;
        LandResult(LandEventType type, Tile* sourceTile, Tile* targetTile, Player* sourcePlayer, Player* targetPlayer, int amount, bool requiresChoice);
        
        LandEventType getType() const;
        Tile* getSourceTile() const;
        Tile* getTargetTile() const;
        Player* getSourcePlayer() const;
        Player* getTargetPlayer() const;
        int getAmount() const;
        bool isRequiresChoice() const;
};
