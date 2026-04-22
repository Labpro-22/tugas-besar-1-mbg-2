#pragma once
#include "Tile.hpp"

using namespace std;

enum PropertyStatus
{
    BANK,
    OWNED,
    MORTGAGED
};

enum PropertyType
{
    STREET,
    RAILROAD,
    UTILITY
};

class Player;

class PropertyTile : public Tile
{
protected:
    Player *owner;
    PropertyStatus status;
    PropertyType propertyType;
    int price; 
    int morgageValue;
public:
    LandResult land(GameContext &G) = 0; // override pure virtual function
    PropertyTile(int idx, string code, string name, int price, int morgageValue);
    bool isOwned() const;
    void setOwner(Player *newOwner);
    void setStatus(PropertyStatus newStatus);

    Player* getOwner() const;
    PropertyStatus getStatus() const;
    PropertyType getPropertyType() const;
    int getPrice() const;
    int getMorgageValue() const;
};

