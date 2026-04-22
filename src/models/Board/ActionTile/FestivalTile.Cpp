#include "FestivalTile.hpp"

FestivalTile::FestivalTile(int idx, string code, string name)
    : ActionTile(idx, code, name, FESTIVAL_TILE) {
}

LandResult FestivalTile::land(GameContext &G){
    return LandResult{LandEventType::DOFESTIVAL, this, nullptr, &G.getCurrentPlayer(), nullptr, 0, false, string("You are at the festival.")};
}
