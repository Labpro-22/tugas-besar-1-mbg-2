#include "FreeParkTile.hpp"

FreeParkTile::FreeParkTile(int idx, string code, string name)
    : ActionTile(idx, code, name, FREE_PARKING_TILE) {
}

LandResult FreeParkTile::land(GameContext &G){
    return LandResult{LandEventType::DONOTHING, this, nullptr, &G.getCurrentPlayer(), nullptr, 0, false, string("You are in free park.")};
}
