#include "ActionTile.hpp"

ActionTile::ActionTile(int idx, string code, string name, ActionType actionType, string color)
    : Tile(idx, code, name, AKSI, color), actionType(actionType) {}

