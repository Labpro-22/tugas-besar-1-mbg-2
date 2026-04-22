
#include "StartTile.hpp"

StartTile::StartTile(int idx, string code, string name)
    : ActionTile(idx, code, name, START_TILE) {
};

LandResult StartTile::land(GameContext &G){
    return LandResult{LandEventType::DONOTHING, this, nullptr, &G.getCurrentPlayer(), nullptr, 0, false, string("You landed on the start tile.")};
};
