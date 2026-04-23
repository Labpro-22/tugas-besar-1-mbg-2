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
}

int StreetTile::getBuildingValue() const  {
    int total = 0;
    if (getHasHotel()){
        total += getHotelCost();
    }
    total += getHouseCost() * getHouseCount();
    return total;
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

vector<int> StreetTile::getRentPrices() const {
    return rentPrices;
}


