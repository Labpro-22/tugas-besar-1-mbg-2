#include "PropertyTile.hpp"

PropertyTile::PropertyTile(int idx, string code, string name, 
    int price, int morgageValue, string color) 
    : Tile(idx, code, name, PROPERTY, color), price(price), morgageValue(morgageValue){
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

string PropertyTile::getDisplayExtra() const {
    return "";
}

vector<string> PropertyTile::getAktaDetailLines(const GameContext& G) const {
    return {};
}

int PropertyTile::getPrice() const {
    return price;
}

int PropertyTile::getMorgageValue() const {
    return morgageValue;
}



