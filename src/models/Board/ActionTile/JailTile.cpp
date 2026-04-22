#include "JailTile.hpp"

JailTile::JailTile(int idx, std::string code, std::string name)
    : ActionTile(idx, code, name, JAIL_TILE) {
}

LandResult JailTile::land(GameContext &G){
    return LandResult{LandEventType::DONOTHING, this, nullptr, &G.getCurrentPlayer(), nullptr, 0, false, string("You are in jail.")};
}