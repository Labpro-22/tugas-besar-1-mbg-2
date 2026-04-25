#include "UtilityTile.hpp"
#include "Dice.hpp"
#include "BankruptcyController.hpp"
#include <algorithm>

using namespace std;


UtilityTile::UtilityTile(int idx, string code, string name, int price, int morgageValue, string color) 
    : PropertyTile(idx, code, name, price, morgageValue, color) {
    this->propertyType = UTILITY;
}

LandResult UtilityTile::land(GameContext &G) {
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
    return LandResult{LandEventType::DONOTHING, this, nullptr, &G.getCurrentPlayer(), nullptr, 0,  false};
}

string UtilityTile::getTypeLabel() const {
    return "UT";
}

vector<string> UtilityTile::getAktaDetailLines(const GameContext& G) const {
    vector<string> lines;
    for (const auto& multiplier : G.getUtilityMultiplierTable()) {
        lines.push_back("Rent Multiplier (" + to_string(multiplier.first) + " owned): " + to_string(multiplier.second) + "x");
    }
    return lines;
}

vector<string> UtilityTile::getRentDetailLines(GameContext& G) const {
    vector<string> lines;
    int ownerUtilities = 0;
    if (owner != nullptr) {
        ownerUtilities = owner->countOwnerUtilities();
    }

    lines.push_back("Condition: " + to_string(ownerUtilities) + " owned");
    Dice dice = G.getDice();
    auto multiplierTable = G.getUtilityMultiplierTable();

    lines.push_back("Rent : M" + to_string(calculateRent(G)));

    lines.push_back("Your Balance: M" + to_string(G.getCurrentPlayer().getBalance()) + "-> M" + to_string(G.getCurrentPlayer().getBalance() - calculateRent(G)));
    lines.push_back(owner->getName() + "'s Balance: M" + to_string(owner->getBalance()) + "-> M" + to_string(owner->getBalance() + calculateRent(G)));
    return lines;
}

int UtilityTile::calculateRent(GameContext& G) const {
    int ownerUtilities = 0;
    if (owner != nullptr) {
        ownerUtilities = owner->countOwnerUtilities();
    }
    auto multiplierTable = G.getUtilityMultiplierTable();
    if (multiplierTable.find(ownerUtilities) != multiplierTable.end()) {
        return G.getDice().getTotal() * multiplierTable.at(ownerUtilities);
    } else {
        return 0;
    }
}

string UtilityTile::getPropertyDisplayInfo() const {
    string status = getStatus() == MORTGAGED ? "MORTGAGED [M]" : getStatus() == OWNED ? "OWNED" : "BANK";
    return getName() + " (" + getCode() + ") M" + to_string(getPrice()) + " " + status;
}