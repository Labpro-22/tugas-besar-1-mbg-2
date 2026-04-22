#pragma once
#include "Tile.hpp"
#include <vector>

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
    PropertyTile(int idx, string code, string name, int price, int morgageValue, string color);
    bool isOwned() const;
    void setOwner(Player *newOwner);
    void setStatus(PropertyStatus newStatus);

    Player* getOwner() const;
    PropertyStatus getStatus() const;
    PropertyType getPropertyType() const;
    virtual string getTypeLabel() const;
    virtual string getDisplayExtra() const;
    virtual vector<string> getAktaDetailLines(const GameContext& G) const;
    virtual vector<string> getRentDetailLines(GameContext& G) const;
    virtual int calculateRent(GameContext& G) const;

    int getPrice() const;
    int getMorgageValue() const;
};

