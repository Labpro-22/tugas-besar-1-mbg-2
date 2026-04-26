#include "FreeParkTile.hpp"

FreeParkTile::FreeParkTile(int idx, string code, string name, string color)
    : ActionTile(idx, code, name, FREE_PARKING_TILE, color) {
}

LandResult FreeParkTile::land(Player &p){
    return LandResult(LandEventType::DONOTHING, this, nullptr, &p, nullptr, 0, false);
}
