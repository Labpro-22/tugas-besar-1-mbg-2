#include "PropertyTile.hpp"

PropertyTile::PropertyTile(int idx, string code, string name, 
    int price, int mortgageValue, string color) 
    : Tile(idx, code, name, PROPERTY, color), price(price), mortgageValue(mortgageValue){
    owner = nullptr;
    type = PROPERTY;
    status = BANK;
};

bool PropertyTile::isOwned() const{
    return this->owner != nullptr;
};

void PropertyTile::setOwner(Player *newOwner){
    this->owner = newOwner;
};

void PropertyTile::setStatus(PropertyStatus newStatus){
    this->status = newStatus;
};

int PropertyTile::getBuildingValue() const {return 0;}

Player* PropertyTile::getOwner() const {
    return owner;
}

PropertyStatus PropertyTile::getStatus() const {
    return status;
}

PropertyType PropertyTile::getPropertyType() const {
    return propertyType;
}


string PropertyTile::getTypeLabel() const {
    return "PR";
}

int PropertyTile::getPrice() const {
    return price;
}

int PropertyTile::getMortgageValue() const {
    return mortgageValue;
}



