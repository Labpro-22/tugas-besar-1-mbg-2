#include "FestivalTile.hpp"

FestivalTile::FestivalTile(int idx, string code, string name, string color)
    : ActionTile(idx, code, name, FESTIVAL_TILE, color) {
}

LandResult FestivalTile::land(Player &p){
    return LandResult(LandEventType::DOFESTIVAL, this, nullptr, &p, nullptr, 0, false);
}
