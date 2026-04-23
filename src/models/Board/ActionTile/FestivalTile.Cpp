#include "FestivalTile.hpp"

FestivalTile::FestivalTile(int idx, string code, string name, string color)
    : ActionTile(idx, code, name, FESTIVAL_TILE, color) {
}

LandResult FestivalTile::land(GameContext &G){
    return LandResult{LandEventType::DOFESTIVAL, this, nullptr, &G.getCurrentPlayer(), nullptr, 0, false};
}
