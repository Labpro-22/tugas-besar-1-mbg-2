#pragma once
#include "PropertyTile.hpp"

using namespace std;

class StreetTile : public PropertyTile
{
private:
    // string colorGroup;  // warna harusnya ga perlu sudah turunan dari kelas Tile
    int houseCost;
    int hotelCost;
    int houseCount;
    bool hasHotel;
    vector<int> rentPrices; 

public:
    StreetTile(int idx, string code, string name, int price, 
        int morgageValue, vector<int> rentPrices, 
        int houseCost, int hotelCost);
    
    LandResult land(GameContext &G) override;
};

