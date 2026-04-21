#include "StreetTile.hpp"
#include "DisplayView.hpp"
#include "BankruptcyController.hpp"

StreetTile::StreetTile(int idx, string code, string name, int price, 
    int morgageValue, vector<int> rentPrices, 
    int houseCost, int hotelCost) : PropertyTile(idx, code, name, price, morgageValue), 
    rentPrices(rentPrices), houseCost(houseCost), hotelCost(hotelCost), houseCount(0), hasHotel(false) {
    this->propertyType = STREET;

}

LandResult StreetTile::land(GameContext &G){
    if (this->status == MORTGAGED) {
        return LandResult{LandEventType::DONOTHING, this, nullptr, G.getCurrentPlayer(), nullptr, 0, false, string("This property is mortgaged. No rent is due.")};
    } else if (this->status == BANK) {
        return LandResult{LandEventType::OFFERBUYPROPERTY, this, nullptr, G.getCurrentPlayer(), nullptr, price, true, string("Do you want to buy ") + this->getName() + string(" for M") + to_string(price) + string("?")};
    } else if (this->status == OWNED) {
        Player* owner = this->owner;
        Player* currentPlayer = G.getCurrentPlayer();
        if (owner == currentPlayer) {
            return LandResult{LandEventType::OFFERUPGRADEPROPERTY, this, nullptr, currentPlayer, owner, 0, false, string("You own this property. No rent is due.")};
        } else {
            return LandResult{LandEventType::PAYRENT, this, nullptr, currentPlayer, owner, 0, false, string("You landed on ") + owner->getName() + string("'s property.")};
        }
    }
}


