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
    int festivalMult = 1;
    int festivalDuration = 0;
    int festivalMult = 1;
    int festivalDuration = 0;
    vector<int> rentPrices; 

public:
    StreetTile(int idx, string code, string name, int price, 
        int morgageValue, vector<int> rentPrices, 
        int houseCost, int hotelCost, string color);
    
    LandResult land(GameContext &G) override;
    
    int getFestivalMult() const;
    int getFestivalDuration() const;
    bool isFestivalActive() const;
    void applyFestival();
    void tickFestival();
    void setHouseCount(int count);
    void setHasHotel(bool value);
    
    int getBuildingValue() const override;
    string getTypeLabel() const override;
    string getDisplayExtra() const override;
    vector<string> getAktaDetailLines(const GameContext& G) const override;
    vector<string> getRentDetailLines(GameContext& G) const override;

    int getHouseCost() const;
    int getHotelCost() const;
    int getHouseCount() const;
    bool getHasHotel() const;
    vector<int> getRentPrices() const;
    int calculateRent(GameContext& G) const override;

};

