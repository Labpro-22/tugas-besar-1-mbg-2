#include <algorithm>

#include "Player.hpp"
#include "PropertyTile.hpp"
#include "SkillCard.hpp"
#include "StreetTile.hpp"
void Player::setName(string name) {
    username = name;
}

void Player::setBalance(int amount) {
    if (amount < 0) {
        balance = 0;
        return;
    }
    balance = amount;
}

void Player::setDoubleCount(int count) {
    if (count < 0) {
        doubleCount = 0;   
        return;
    }
    doubleCount = count;   
}

void Player::setPosition(int pos) {
    currentPosition = pos;
}

void Player::setStatus(PlayerStatus status) {
    this->status = status;
}

void Player::setJailTurns(int currentJailAttempt) {
    if (currentJailAttempt < 0) {
        jailTurns = 0;
        return;
    }
    jailTurns = currentJailAttempt;
}

void Player::applyShield() {
    this->shieldActive = true;
}

void Player::applyDiscount(int percentage) {
    this->rentDiscount = percentage;
}

Player Player::operator+(int amount) {
    Player result = *this;
    result.balance += amount;
    return result;
}

Player& Player::operator+=(int amount) {
    balance += amount;
    return *this;
}

Player Player::operator-(int amount) {
    Player result = *this;
    result.balance -= amount;
    return result;
}

Player& Player::operator-=(int amount) {
    balance -= amount;
    return *this;
}

bool Player::operator>=(int amount) const {
    return balance >= amount;
}

bool Player::operator<(int amount) const {
    return balance < amount;
}

bool Player::canAfford(int amount) const {
    return balance >= amount;
}

bool Player::operator>(const Player &other) const {
    return balance > other.balance;
}

bool Player::operator<(const Player &other) const {
    return balance < other.balance;
}

bool Player::operator==(const Player &other) const {
    return balance == other.balance;
}

bool Player::operator>=(const Player &other) const {
    return balance >= other.balance;
}

bool Player::operator<=(const Player &other) const {
    return balance <= other.balance;
}

void Player::addProperty(PropertyTile *property) {
    if (property == nullptr) {
        return;
    }

    auto it = std::find(ownedProperties.begin(), ownedProperties.end(), property);
    if (it == ownedProperties.end()) {
        ownedProperties.push_back(property);
    }
}

void Player::removeProperty(PropertyTile *property) {
    auto it = std::find(ownedProperties.begin(), ownedProperties.end(), property);
    if (it != ownedProperties.end()) {
        ownedProperties.erase(it);
    }
}

void Player::addCardToHand(SkillCard *card) {
    skillCards.push_back(card);
}

SkillCard* Player::useSkillCard(int idx) {
    if (idx < 0 || idx >= skillCards.size()) {
        return nullptr; 
    }

    SkillCard* cardToUse = skillCards[idx];
    skillCards.erase(skillCards.begin() + idx);
    return cardToUse;
}

SkillCard* Player::dropSkillCard(int idx) {
    return useSkillCard(idx);
}

bool Player::hasAnySkillCard() const {
    return !skillCards.empty(); 
}

int Player::searchJailFreeCard() const {
    for (int i = 0; i < skillCards.size(); i++) {
        if (skillCards[i]->getName() == "JailFreeCard") { 
            return i;
        }
    }
    return -1;
}

vector<SkillCard *> Player::getSkillCard(){
    return skillCards;
}

int Player::getSkillCardCount() const {
    return skillCards.size();
}

bool Player::canHoldMoreSkillCards() const {
    return getSkillCardCount() < MAX_CARDS;
}

string Player::getName() const {
    return username;
}

int Player::getBalance() const {
    return balance;
}

int Player::getPosition() const {
    return currentPosition;
}

PlayerStatus Player::getStatus() const {
    return status;
}

int Player::getJailTurns() const {
    return jailTurns;
}

vector<PropertyTile*>& Player::getOwnedProperties(){
    return ownedProperties;
}

bool Player::hasShield() const {
    return this->shieldActive;
}

int Player::getRentDiscount() const {
    return this->rentDiscount;
}

int Player::totalPropertyPrice() const {
    int output = 0;
    for (PropertyTile *property : ownedProperties) {
        if (property == nullptr) {
            continue;
        }
        output += property->getPrice();
    }
    return output;
}
int Player::totalBuildingValue() const {
    int output = 0;
    for (PropertyTile *property : ownedProperties) {
        if (property == nullptr) {
            continue;
        }
        output += property->getBuildingValue();
    }
    return output;
}

int Player::totalWealth() const
{
    return balance + totalPropertyPrice() + totalBuildingValue() / 2;
}

int Player::countOwnerRailroads() const {
    int count = 0;
    for (PropertyTile *property : ownedProperties) {
        if (property == nullptr) {
            continue;
        }

        string code = property->getCode();
        if (code == "GBR" || code == "STB" || code == "TUG" || code == "GUB") {
            count++;
        }
    }
    return count;
}

int Player::countOwnerUtilities() const {
    int count = 0;
    for (PropertyTile *property : ownedProperties) {
        if (property == nullptr) {
            continue;
        }

        string code = property->getCode();
        if (code == "PLN" || code == "PAM") {
            count++;
        }
    }
    return count;
}

vector<PropertyTile*> Player::getMortgagedProperties() {
    vector<PropertyTile*> mortgagedProperties;
    for (PropertyTile* property : ownedProperties) {
        if (property != nullptr && property->getStatus() == MORTGAGED) {
            mortgagedProperties.push_back(property);
        }
    }
    return mortgagedProperties;
}

map<string, vector<PropertyTile*>> Player::getMapColorOwnedProperty() {
    map<string, vector<PropertyTile*>> colorMap;
    for (PropertyTile* property : ownedProperties) {
        if (property != nullptr) {
            colorMap[property->getColor()].push_back(property);
        }
    }
    return colorMap;
}

map<string, vector<StreetTile*>> Player::getColorOwnedStreetTile() {
    map<string, vector<StreetTile*>> StreetTileMap;
    for (PropertyTile* property : ownedProperties) {
        StreetTile* street = dynamic_cast<StreetTile*>(property);
        if (street != nullptr && street->getStatus() != MORTGAGED && street->getTypeLabel() == "ST") {
            StreetTileMap[street->getColor()].push_back(street);
        }
    }
    return StreetTileMap;
}

vector<StreetTile*> Player::getStreetTileByColor(const string& color) {
    vector<StreetTile*> streetTiles;
    for (PropertyTile* property : ownedProperties) {
        StreetTile* street = dynamic_cast<StreetTile*>(property);
        if (street != nullptr && street->getColor() == color) {
            streetTiles.push_back(street);
        }
    }
    return streetTiles;
}

map<string, vector<PropertyTile*>> Player::getUnmortgagedGroups() {
    map<string, vector<PropertyTile*>> unmortgagedGroups;
    for (PropertyTile* property : ownedProperties) {
        if (property != nullptr && property->getStatus() != MORTGAGED) {
            unmortgagedGroups[property->getColor()].push_back(property);
        }
    }
    return unmortgagedGroups;
}

void Player::resetBuffs() {
    this->shieldActive = false;
    this->rentDiscount = 0;
}