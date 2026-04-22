#include <algorithm>

#include "Player.hpp"
#include "PropertyTile.hpp"
#include "SkillCard.hpp"

void Player::move(int steps) {
    const int boardSize = 40;
    int nextPosition = (currentPosition + steps) % boardSize;
    if (nextPosition < 0) {
        nextPosition += boardSize;
    }
    currentPosition = nextPosition;
}

void Player::setPosition(int pos) {
    const int boardSize = 40;
    int normalizedPosition = pos % boardSize;
    if (normalizedPosition < 0) {
        normalizedPosition += boardSize;
    }
    currentPosition = normalizedPosition;
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

bool Player::operator==(Player &other) const {
    return balance == other.balance;
}

bool Player::operator>=(Player &other) const {
    return balance >= other.balance;
}

bool Player::operator<=(Player &other) const {
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

void Player::addSkillCard(SkillCard *card) {
    if (card == nullptr) {
        return;
    }

    auto it = std::find(skillCards.begin(), skillCards.end(), card);
    if (it == skillCards.end()) {
        skillCards.push_back(card);
    }
}

void Player::removeSkillCard(SkillCard *card) {
    if (card == nullptr) {
        return;
    }

    auto it = std::find(skillCards.begin(), skillCards.end(), card);
    if (it != skillCards.end()) {
        skillCards.erase(it);
    }
}

SkillCard *Player::getSkillCard(const string &cardName) const {
    auto it = std::find_if(skillCards.begin(), skillCards.end(), [&](const SkillCard *card) {
        return card != nullptr && card->getName() == cardName;
    });

    if (it == skillCards.end()) {
        return nullptr;
    }

    return *it;
}

bool Player::hasSkillCard(const SkillCard *card) const {
    if (card == nullptr) {
        return false;
    }

    return std::find(skillCards.begin(), skillCards.end(), card) != skillCards.end();
}

bool Player::hasSkillCard(const string &cardName) const {
    return std::find_if(skillCards.begin(), skillCards.end(), [&](const SkillCard *card) {
        return card != nullptr && card->getName() == cardName;
    }) != skillCards.end();
}

int Player::getSkillCardCount() const {
    int count = 0;
    for (const SkillCard *card : skillCards) {
        if (card != nullptr) {
            count++;
        }
    }
    return count;
}

bool Player::canHoldMoreSkillCards() const {
    const int maxHandSize = 3;
    return getSkillCardCount() < maxHandSize;
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
