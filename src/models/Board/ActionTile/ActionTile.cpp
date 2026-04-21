#include "ActionTile.hpp"

ActionTile::ActionTile(int idx, string code, string name, ActionType actionType)
    : Tile(idx, code, name, AKSI), actionType(actionType) {}

