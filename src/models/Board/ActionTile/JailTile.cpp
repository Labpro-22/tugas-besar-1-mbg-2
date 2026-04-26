#include "JailTile.hpp"

JailTile::JailTile(int idx, std::string code, std::string name, string color)
    : ActionTile(idx, code, name, JAIL_TILE, color) {
}

LandResult JailTile::land(Player &p){
    return LandResult(LandEventType::DONOTHING, this, nullptr, &p, nullptr, 0, false);
}