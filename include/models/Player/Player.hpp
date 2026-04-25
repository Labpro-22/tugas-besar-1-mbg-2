#pragma once
#include <string>
#include <list>
#include <vector>
#include <map>

using namespace std;

class PropertyTile;
class SkillCard;
class StreetTile;

enum PlayerStatus
{
    ACTIVE,
    JAILED,
    BANKRUPT
};
class Player
{
private:
    string username;
    int balance = 0;
    int currentPosition = 0;
    PlayerStatus status = ACTIVE;
    int jailTurns = 0;
    int doubleCount = 0;
    vector<PropertyTile*> ownedProperties;
    vector<SkillCard*> skillCards;
    bool shieldActive = false;
    int rentDiscount = 0;
    int MAX_CARDS = 3;
public:

    Player() = default;
    Player(string name);
    void setName(string name);
    void setBalance(int amount);
    void setDoubleCount(int count);
    void setPosition(int pos);
    void setStatus(PlayerStatus status);
    void setJailTurns(int currentJailAttempt);
    void applyShield();
    void applyDiscount(int percentage);

    Player operator+(int amount); // add money
    Player& operator+=(int amount); // add money
    Player operator-(int amount); // reduce money
    Player& operator-=(int amount); // reduce money
    bool operator>=(int amount) const; // can afford suggested by me
    bool operator<(int amount) const; // can't afford

    bool canAfford(int amount) const;
    
    bool operator>(const Player &other) const; // compare richness
    bool operator<(const Player &other) const; // compare richness
    bool operator==(const Player &other) const; // compare richness
    bool operator>=(const Player &other) const; // compare richness
    bool operator<=(const Player &other) const; // compare richness

    void addProperty(PropertyTile* property);
    void removeProperty(PropertyTile* property);

    void addCardToHand(SkillCard* card);
    vector<SkillCard*> getSkillCard();
    SkillCard* useSkillCard(int idx);
    SkillCard* dropSkillCard(int idx);
    bool hasAnySkillCard() const;
    int searchJailFreeCard() const;
    int getSkillCardCount() const;
    bool canHoldMoreSkillCards() const;

    string getName() const;
    int getBalance() const;
    int getPosition() const;
    PlayerStatus getStatus() const;
    int getJailTurns() const;
    bool hasShield() const;
    int getRentDiscount() const;

    vector<PropertyTile*>& getOwnedProperties();
  
    int totalWealth() const;
    int totalPropertyPrice() const;
    int totalBuildingValue() const;

    int countOwnerRailroads() const; // Untuk railroad tapi perlu di bahas
    int countOwnerUtilities() const; // Untuk utility tapi perlu di bahas

    vector<PropertyTile*> getMortgagedProperties();
    vector<PropertyTile*> getUnmortgagedProperties();
    vector<StreetTile*> getStreetTileByColor(const string& color);
    
    map<string, vector<PropertyTile*>> getUnmortgagedGroups();
    map<string, vector<PropertyTile*>> getMapColorOwnedProperty();
    map<string, vector<StreetTile*>> getColorOwnedStreetTile();

    void resetBuffs();
};

