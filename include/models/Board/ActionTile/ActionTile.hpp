#pragma once
#include "Tile.hpp"

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
ActionTile(int idx, string code, string name, ActionType actionType); 
// Tile type ga perlu ada jadi attribute tapi waktu implementasi tinggal constructor aja, 
// misal ActionTile(int idx, string code, string name, ActionType actionType) : Tile(idx, code, name, TileType::AKSI), actionType(actionType) {}
    virtual void land(Player &p) = 0; // override pure virtual function
};

