#pragma once
#include <string>
#include <list>
#include <vector>

using namespace std;

class PropertyTile;
class SkillCard;

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
    list<PropertyTile*> ownedProperties;
    vector<SkillCard*> skillCards;
public:

    Player() = default;
    void move(int steps);
    void setName(string name);
    void setBalance(int amount);
    void setDoubleCount(int count);
    void setPosition(int pos);
    

    Player operator+(int amount); // add money
    Player& operator+=(int amount); // add money
    Player operator-(int amount); // reduce money
    Player& operator-=(int amount); // reduce money
    bool operator>=(int amount) const; // can afford suggested by me
    bool operator<(int amount) const; // can't afford

    bool canAfford(int amount) const;
    
    bool operator>(Player &other) const; // compare richness
    bool operator<(Player &other) const; // compare richness
    bool operator==(Player &other) const; // compare richness
    bool operator>=(Player &other) const; // compare richness
    bool operator<=(Player &other) const; // compare richness

    void addProperty(PropertyTile* property);
    void removeProperty(PropertyTile* property);

    void addSkillCard(SkillCard* card);
    void removeSkillCard(SkillCard* card);

    
    string getName();
    int getBalance();
    int getPosition();
    PlayerStatus getStatus();
    int getJailTurns();
    int countOwnerRailroads() const; // Untuk railroad tapi perlu di bahas
    int countOwnerUtilities() const; // Untuk utility tapi perlu di bahas
};

