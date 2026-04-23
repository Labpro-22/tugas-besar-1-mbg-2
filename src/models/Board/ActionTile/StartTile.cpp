
#include "StartTile.hpp"

StartTile::StartTile(int idx, string code, string name, string color)
    : ActionTile(idx, code, name, START_TILE, color) {
};

LandResult StartTile::land(GameContext &G){
    return LandResult{LandEventType::DONOTHING, this, nullptr, &G.getCurrentPlayer(), nullptr, 0, false};
};
