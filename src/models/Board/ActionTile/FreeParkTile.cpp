#include "FreeParkTile.hpp"

FreeParkTile::FreeParkTile(int idx, string code, string name, string color)
    : ActionTile(idx, code, name, FREE_PARKING_TILE, color) {
}

LandResult FreeParkTile::land(GameContext &G){
    return LandResult{LandEventType::DONOTHING, this, nullptr, &G.getCurrentPlayer(), nullptr, 0, false};
}
