#include "GoToJailTile.hpp"

GoToJailTile::GoToJailTile(int idx, string code, string name, string color)
    : ActionTile(idx, code, name, GOTO_JAIL_TILE, color) {
};

LandResult GoToJailTile::land(GameContext &G){
    return LandResult{LandEventType::GOTOJAIL, this, nullptr, &G.getCurrentPlayer(), nullptr, 0, false};
}