#pragma once
#include "PropertyTile.hpp"

class StreetTile : public PropertyTile
{
private:
    string colorGroup;  // warna harusnya ga perlu sudah turunan dari kelas Tile
    int houseCost;
    int hotelCost;
    int houseCount;
    bool hasHotel;
public:
    StreetTile(int idx, string code, string name, int price, 
        int morgageValue, vector<int> rentPrices, 
        string colorGroup, int houseCost, int hotelCost);
    
    PropertyStatus land(Player &p) override;
};

