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

int StreetTile::getFestivalStack() const {
    return festivalState.getStacks();
}

int StreetTile::getFestivalTurn() const {
    return festivalState.getTurnsLeft();
}

void StreetTile::setFestivalState(FestivalState newState) {
    festivalState = newState;
}

bool StreetTile::isFestivalActive() const {
    return festivalState.getTurnsLeft() > 0 && festivalState.getStacks() > 1;
}

void StreetTile::applyFestival() {
    festivalState.activateFestival();
}

void StreetTile::playerReenterFestival(){
    festivalState.resetDuration();
    festivalState.increaseStack();
}

void StreetTile::decreaseFestivalTurn(){
    festivalState.decrementTurn();
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

vector<string> StreetTile::getAktaDetailLines(const GameContext& G) const {
    vector<string> lines;
    int level = 0;
    for (int rentPrice : rentPrices) {
        if (level == 0) {
            lines.push_back("Rent Fee (No Building): M" + to_string(rentPrice));
        } else if (level == 5) {
            lines.push_back("Rent Fee (Hotel): M" + to_string(rentPrice));
        } else {
            lines.push_back("Rent Fee (" + to_string(level) + " House(s)): M" + to_string(rentPrice));
        }
        level++;
    }
    lines.push_back("-");
    lines.push_back("House Cost       : M" + to_string(houseCost));
    lines.push_back("Hotel Cost       : M" + to_string(hotelCost));
    if (isFestivalActive()) {
        lines.push_back("-");
        lines.push_back("FESTIVAL ACTIVE!");
        lines.push_back("Festival Multiplier: " + to_string(festivalState.getStacks()) + "x");
        lines.push_back("Festival Duration: " + to_string(festivalState.getTurnsLeft()) + " turns remaining");
    }
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

string StreetTile::getPropertyDisplayInfo() const {
    string status = getStatus() == MORTGAGED ? "MORTGAGED [M]" : getStatus() == OWNED ? "OWNED" : "BANK";
    string info = getName() + " (" + getCode() + ")";
    if (hasHotel) {
        info += " Hotel";
    } else if (houseCount > 0) {
        info += to_string(houseCount) + " House(s)";
    }

    if (isOwned()) {
        info += " M" + to_string(getPrice());
    } else {
        info += " M" + to_string(getPrice());
    }
    info += " " + status;
    
    return info;
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

void StreetTile::demolishBuilding() {
    houseCount = 0;
    hasHotel = false;
}
