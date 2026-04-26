#pragma once
#include "Tile.hpp"
#include "LandResult.hpp"
#include "Player.hpp"
#include <vector>

using namespace std;

class GameContext;
struct LandResult;
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
    int mortgageValue;
public:
    LandResult land(Player &p) = 0; // override pure virtual function
    PropertyTile(int idx, string code, string name, int price, int mortgageValue, string color);
    bool isOwned() const;
    void setOwner(Player *newOwner);
    void setStatus(PropertyStatus newStatus);

    Player* getOwner() const;
    PropertyStatus getStatus() const;
    PropertyType getPropertyType() const;
    virtual int getBuildingValue() const;
    virtual string getTypeLabel() const = 0;
    virtual string getPropertyDisplayInfo() const = 0;
    virtual vector<string> getAktaDetailLines(const GameContext& G) const = 0;
    virtual vector<string> getRentDetailLines(GameContext& G) const = 0;
    virtual int calculateRent(GameContext& G) const = 0;
    int getPrice() const;
    int getMortgageValue() const;
};

