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
    int balance;
    int currentPosition;
    PlayerStatus status;
    int jailTurns;
    int doubleCount;
    list<PropertyTile*> ownedProperties;
    vector<SkillCard*> skillCards;
    const int MAX_CARDS = 3;
public:
    void move(int steps);
    void setPosition(int pos);
    void setStatus(PlayerStatus status);
    void setJailTurns(int currentJailAttempt);
    
    
    Player operator+(int amount); // add money
    Player& operator+=(int amount); // add money
    Player operator-(int amount); // reduce money
    Player& operator-=(int amount); // reduce money
    bool operator>=(int amount) const; // can afford suggested by me
    bool operator<(int amount) const; // can't afford

    bool canAfford(int amount) const;
    
    bool operator>(const Player &other) const; // compare richness
    bool operator<(const Player &other) const; // compare richness
    bool operator==(Player &other) const; // compare richness
    bool operator>=(Player &other) const; // compare richness
    bool operator<=(Player &other) const; // compare richness

    void addProperty(PropertyTile* property);
    void removeProperty(PropertyTile* property);

    void addCardToHand(SkillCard* card);
    SkillCard* useSkillCard(int idx);
    SkillCard* dropSkillCard(int idx);
    bool hasAnySkillCard() const;
    int getSkillCardCount() const;
    bool canHoldMoreSkillCards() const;

    
    string getName() const;
    int getBalance() const;
    int getPosition() const;
    PlayerStatus getStatus() const;
    int getJailTurns() const;
    int countOwnerRailroads() const; // Untuk railroad tapi perlu di bahas
    int countOwnerUtilities() const; // Untuk utility tapi perlu di bahas
};

