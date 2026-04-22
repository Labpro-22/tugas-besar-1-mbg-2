#pragma once
#include "Tile.hpp"

using namespace std;

enum ActionType
{
    CARD_TILE,
    JAIL_TILE,
    FREE_PARKING_TILE,
    TAX_TILE,
    FESTIVAL_TILE,
    GOTO_JAIL_TILE,
    START_TILE,
};

class ActionTile : public Tile
{
protected:
    ActionType actionType; 
    
public:
    ActionTile(int idx, string code, string name, ActionType actionType, string color = "DEFAULT"); 
    virtual LandResult land(GameContext &G) = 0; // override pure virtual function
};

