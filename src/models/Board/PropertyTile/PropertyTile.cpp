#include "PropertyTile.hpp"

PropertyTile::PropertyTile(int idx, string code, string name, 
    int price, int morgageValue) : Tile(idx, code, name, PROPERTY), price(price), morgageValue(morgageValue){
    owner = nullptr;
    type = PROPERTY;
    status = BANK;
};

bool PropertyTile::isOwned() const{
    return this->owner != nullptr;
};

int PropertyTile::getPrice() const {
    return price;
}

int PropertyTile::getMorgageValue() const {
    return morgageValue;
}

PropertyStatus PropertyTile::getStatus() const {
    return status;
}

void PropertyTile::setOwner(Player *newOwner){
    this->owner = newOwner;
};

void PropertyTile::setStatus(PropertyStatus newStatus){
    this->status = newStatus;
};
