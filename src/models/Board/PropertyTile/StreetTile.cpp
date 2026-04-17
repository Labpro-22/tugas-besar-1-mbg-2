#include "StreetTile.hpp"
#include "DisplayView.hpp"
#include "BankruptcyController.hpp"

StreetTile::StreetTile(int idx, string code, string name, int price, 
    int morgageValue, vector<int> rentPrices, 
    int houseCost, int hotelCost) : PropertyTile(idx, code, name, price, morgageValue, rentPrices), 
    houseCost(houseCost), hotelCost(hotelCost), houseCount(0), hasHotel(false) {\
    this->PropertyType = STREET;

}

void StreetTile::land(Player &p){
    if (!isOwned() && this->status == BANK) {
        DisplayView::renderAkta(*this);
    } else if (owner != &p && status == OWNED) {
        Player *ownerPlayer = owner;
        int rentIndex = houseCount; 
        if (hasHotel) {
            rentIndex = rentPrices.size() - 1; 
        }
        int rent = rentPrices[rentIndex];
        if (!p.canAfford(rent)) {
            BankruptcyController::checkSolvency(p, rent); // INI JELEK PERLU DIUBAH
        }
        p -= rent;
        *ownerPlayer += rent;
    }
}


