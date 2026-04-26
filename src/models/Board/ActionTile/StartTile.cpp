
#include "StartTile.hpp"

StartTile::StartTile(int idx, string code, string name, string color)
    : ActionTile(idx, code, name, START_TILE, color) {
};

LandResult StartTile::land(Player &p){
    return LandResult(LandEventType::DONOTHING, this, nullptr, &p, nullptr, 0, false);
};
