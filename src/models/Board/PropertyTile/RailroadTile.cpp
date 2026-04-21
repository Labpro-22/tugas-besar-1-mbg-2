#include "RailroadTile.hpp"

RailroadTile::RailroadTile(int idx, string code, string name, 
    int price, int morgageValue) : 
    PropertyTile(idx, code, name, price, morgageValue) {
    this->propertyType = RAILROAD;
}

LandResult RailroadTile::land(GameContext &G) {
    if(this->status == MORTGAGED) {
        return LandResult{LandEventType::DONOTHING, this, nullptr, G.getCurrentPlayer(), nullptr, 0, false, string("This railroad is mortgaged. No rent is due.")};
    }else if(this->status == BANK) {
        return LandResult{LandEventType::GIVEPROPERTY, this, nullptr, G.getCurrentPlayer(), nullptr, price, true, string("This railroad is not owned. So you got it for free.")};
    }else if(this->status == OWNED) {
        Player* owner = this->owner;
        Player* currentPlayer = G.getCurrentPlayer();
        if(owner == currentPlayer) {
            return LandResult{LandEventType::DONOTHING, this, nullptr, currentPlayer, owner, 0, false, string("You own this railroad. No rent is due.")};
        } else {
            return LandResult{LandEventType::PAYRENT, this, nullptr, currentPlayer, owner, 0, false, string("You landed on ") + owner->getName() + string("'s railroad. Calculate rent based on number of railroads owned.")};
        }
    }
    return LandResult{LandEventType::DONOTHING, this, nullptr, G.getCurrentPlayer(), nullptr, 0, false, string("Unexpected property status.")};
}
