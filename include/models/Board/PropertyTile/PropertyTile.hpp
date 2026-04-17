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

class PropertyTile : public Tile
{
protected:
    Player *owner;
    PropertyStatus status;
    PropertyType PropertyType; // diubah dari Type menjadi PropertyType untuk membedakan dengan TileType di kelas Tile
    int price; 
    int morgageValue;
    vector<int> rentPrices; 
public:
    virtual PropertyStatus land(Player &p) = 0; // override pure virtual function
    PropertyTile(int idx, string code, string name, int price, int morgageValue, vector<int> rentPrices);
    bool isOwned() const;
    void setOwner(Player *newOwner);
    void setStatus(PropertyStatus newStatus);
};

