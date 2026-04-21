#include "GoToJailTile.hpp"

GoToJailTile::GoToJailTile(int idx, string code, string name)
    : ActionTile(idx, code, name, GOTO_JAIL_TILE) {
};

LandResult GoToJailTile::land(GameContext &G){
    return LandResult{LandEventType::GOTOJAIL, this, nullptr, G.getCurrentPlayer(), nullptr, 0, false, string("You landed on Go To Jail tile. You need to go to jail.")};
}