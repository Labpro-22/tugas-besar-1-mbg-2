#include "RailroadTile.hpp"

RailroadTile::RailroadTile(int idx, string code, string name, 
    int price, int morgageValue, string color) : 
    PropertyTile(idx, code, name, price, morgageValue, color) {
    this->propertyType = RAILROAD;
}

LandResult RailroadTile::land(GameContext &G) {
    if(this->status == MORTGAGED) {
        return LandResult{LandEventType::DONOTHING, this, nullptr, &G.getCurrentPlayer(), nullptr, 0, false};
    }else if(this->status == BANK) {
        return LandResult{LandEventType::GIVEPROPERTY, this, nullptr, &G.getCurrentPlayer(), nullptr, price, true};
    }else if(this->status == OWNED) {
        Player* owner = this->owner;
        Player* currentPlayer = &G.getCurrentPlayer();
        if(owner == currentPlayer) {
            return LandResult{LandEventType::DONOTHING, this, nullptr, currentPlayer, owner, 0, false};
        } else {
            return LandResult{LandEventType::PAYRENT, this, nullptr, currentPlayer, owner, 0, false};
        }
    }
    return LandResult{LandEventType::DONOTHING, this, nullptr, &G.getCurrentPlayer(), nullptr, 0, false};
}
