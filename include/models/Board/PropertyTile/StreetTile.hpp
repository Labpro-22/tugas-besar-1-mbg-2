#pragma once
#include "PropertyTile.hpp"

using namespace std;

class GameContext;
class LandResult;

class StreetTile : public PropertyTile
{
private:
    int houseCost;
    int hotelCost;
    int houseCount;
    bool hasHotel;
    int festivalMult = 1;
    int festivalDuration = 0;
    vector<int> rentPrices; 

public:
    StreetTile(int idx, string code, string name, int price, 
        int mortgageValue, vector<int> rentPrices, 
        int houseCost, int hotelCost, string color);
    
    LandResult land(GameContext &G) override;
    
    int getFestivalStack() const;
    int getFestivalTurn() const;
    bool isFestivalActive() const;
    void applyFestival();
    void playerReenterFestival();
    void decreaseFestivalTurn();
    void setHouseCount(int count);
    void setHasHotel(bool value);
    
    int getBuildingValue() const override;
    string getTypeLabel() const override;
    string getDisplayExtra() const override;
    string getPropertyDisplayInfo() const override;
    vector<string> getAktaDetailLines(const GameContext& G) const override;
    vector<string> getRentDetailLines(GameContext& G) const override;

    int getHouseCost() const;
    int getHotelCost() const;
    int getHouseCount() const;
    bool getHasHotel() const;
    vector<int> getRentPrices() const;
    int calculateRent(GameContext& G) const override;
};