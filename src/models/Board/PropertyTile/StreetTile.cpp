#include "StreetTile.hpp"
#include "DisplayView.hpp"
#include "BankruptcyController.hpp"

StreetTile::StreetTile(int idx, string code, string name, int price, 
    int morgageValue, vector<int> rentPrices, 
    int houseCost, int hotelCost, string color) : PropertyTile(idx, code, name, price, morgageValue, color), 
    rentPrices(rentPrices), houseCost(houseCost), hotelCost(hotelCost), houseCount(0), hasHotel(false) {
    this->propertyType = STREET;

}

LandResult StreetTile::land(GameContext &G){
    if (this->status == MORTGAGED) {
        return LandResult{LandEventType::DONOTHING, this, nullptr, &G.getCurrentPlayer(), nullptr, 0, false};
    } else if (this->status == BANK) {
        return LandResult{LandEventType::OFFERBUYPROPERTY, this, nullptr, &G.getCurrentPlayer(), nullptr, price, true};
    } else if (this->status == OWNED) {
        Player* owner = this->owner;
        Player* currentPlayer = &G.getCurrentPlayer();
        if (owner == currentPlayer) {
            return LandResult{LandEventType::DONOTHING, this, nullptr, currentPlayer, owner, 0, false};
        } else {
            return LandResult{LandEventType::PAYRENT, this, nullptr, currentPlayer, owner, 0, false};
        }
    }
    return LandResult{LandEventType::DONOTHING, this, nullptr, &G.getCurrentPlayer(), nullptr, 0, false};
}

int StreetTile::getFestivalMult() const {
    return festivalMult;
}

int StreetTile::getFestivalDuration() const {
    return festivalDuration;
}

bool StreetTile::isFestivalActive() const {
    return festivalDuration > 0 && festivalMult > 1;
}

void StreetTile::applyFestival() {
    if (festivalDuration <= 0) {
        festivalMult = 2;
        festivalDuration = 3;
        return;
    }

    festivalMult += 1;
    festivalDuration = 3;
}

void StreetTile::tickFestival() {
    if (festivalDuration <= 0) {
        festivalDuration = 0;
        festivalMult = 1;
        return;
    }

    festivalDuration -= 1;
    if (festivalDuration <= 0) {
        festivalDuration = 0;
        festivalMult = 1;
    }
}

int StreetTile::getBuildingValue() const  {
    int total = 0;
    if (getHasHotel()){
        total += getHotelCost();
    }
    total += getHouseCost() * getHouseCount();
    return total;
}

string StreetTile::getTypeLabel() const {
    return "ST";
}

string StreetTile::getDisplayExtra() const {
    if (hasHotel) {
        return " Hotel";
    }
    if (houseCount > 0) {
        return " " + to_string(houseCount) + " houses";
    }
    return "";
}

vector<string> StreetTile::getAktaDetailLines(const GameContext& G) const {
    vector<string> lines;
    int level = 1;
    for (int rentPrice : rentPrices) {
        lines.push_back("Rent Fee (" + to_string(level++) + " Houses): M" + to_string(rentPrice));
    }
    lines.push_back("-");
    lines.push_back("House Cost       : M" + to_string(houseCost));
    lines.push_back("Hotel Cost       : M" + to_string(hotelCost));
    return lines;
}

vector<string> StreetTile::getRentDetailLines(GameContext& G) const {
    vector<string> lines;
    int rentIndex = houseCount;
    if (hasHotel) {
        rentIndex = 5; // index untuk hotel
        lines.push_back("Condition: Hotel");
    } else {
        lines.push_back("Condition: " + to_string(rentIndex) + " house(s)");
    }

    lines.push_back("Rent : M" + to_string(calculateRent(G)));

    lines.push_back("Your Balance: M" + to_string(G.getCurrentPlayer().getBalance()) + " -> M" + to_string(G.getCurrentPlayer().getBalance() - calculateRent(G)));
    lines.push_back(owner->getName() + "'s Balance: M" + to_string(owner->getBalance()) + " -> M" + to_string(owner->getBalance() + calculateRent(G)));
    return lines;
}

int StreetTile::calculateRent(GameContext& G) const {
    int rentIndex = houseCount;
    if (hasHotel) {
        rentIndex = 5; // index untuk hotel
    }
    if (rentIndex < rentPrices.size()) {
        return rentPrices[rentIndex];
    } else {
        return 0;
    }
}

int StreetTile::getHouseCost() const {
    return houseCost;
}

int StreetTile::getHotelCost() const {
    return hotelCost;
}

int StreetTile::getHouseCount() const {
    return houseCount;
}

bool StreetTile::getHasHotel() const {
    return hasHotel;
}

void StreetTile::setHouseCount(int count) {
    if (count < 0) {
        houseCount = 0;
        return;
    }
    houseCount = count;
}

void StreetTile::setHasHotel(bool value) {
    hasHotel = value;
}

vector<int> StreetTile::getRentPrices() const {
    return rentPrices;
}


