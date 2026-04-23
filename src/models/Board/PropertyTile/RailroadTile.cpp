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

string RailroadTile::getTypeLabel() const {
    return "RR";
}

vector<string> RailroadTile::getAktaDetailLines(const GameContext& G) const {
    vector<string> lines;
    for (const auto& rentPrice : G.getRailroadRentTable()) {
        lines.push_back("Rent Fee (" + to_string(rentPrice.first) + " owned): M" + to_string(rentPrice.second));
    }
    return lines;
}

vector<string> RailroadTile::getRentDetailLines(GameContext& G) const {
    vector<string> lines;
    int ownerRailroads = 0;
    if (owner != nullptr) {
        ownerRailroads = owner->countOwnerRailroads();
    }
    lines.push_back("Condition: " + to_string(ownerRailroads) + " owned");
    
    lines.push_back("Rent : M" + to_string(calculateRent(G)));

    lines.push_back("Your Balance: M" + to_string(G.getCurrentPlayer().getBalance()) + "-> M" + to_string(G.getCurrentPlayer().getBalance() - calculateRent(G)));
    lines.push_back(owner->getName() + "'s Balance: M" + to_string(owner->getBalance()) + "-> M" + to_string(owner->getBalance() + calculateRent(G)));

    return lines;
}

int RailroadTile::calculateRent(GameContext& G) const {
    int ownerRailroads = 0;
    if (owner != nullptr) {
        ownerRailroads = owner->countOwnerRailroads();
    }
    auto rentTable = G.getRailroadRentTable();
    if (rentTable.find(ownerRailroads) != rentTable.end()) {
        return rentTable.at(ownerRailroads);
    } else {
        return 0;
    }
}

string RailroadTile::getPropertyDisplayInfo() const {
    string status = this->status == MORTGAGED ? "MORTGAGED [M]" : this->status == OWNED ? "OWNED" : "BANK";
    return getName() + " (" + getCode() + ") M" + to_string(getPrice()) + " " + status;
}
