#pragma once
#include <string>
#include <list>
#include <vector>
#include "PropertyTile.hpp"
#include "SkillCard.hpp"

using namespace std;

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
    int balance;
    int currentPosition;
    PlayerStatus status;
    int jailTurns;
    int doubleCount;
    list<PropertyTile*> ownedProperties;
    vector<SkillCard*> skillCards;
public:
    void move(int steps);
    void setPosition(int pos);
    

    Player operator+(int amount); // add money
    Player operator+=(int amount); // add money
    Player operator-(int amount); // reduce money
    Player operator-=(int amount); // reduce money
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
};

