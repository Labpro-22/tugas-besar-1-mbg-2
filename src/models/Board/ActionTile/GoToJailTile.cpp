#include "GoToJailTile.hpp"

GoToJailTile::GoToJailTile(int idx, string code, string name, string color)
    : ActionTile(idx, code, name, GOTO_JAIL_TILE, color) {
};

LandResult GoToJailTile::land(Player &p){
    return LandResult(LandEventType::GOTOJAIL, this, nullptr, &p, nullptr, 0, false);
}