#include "UtilityTile.hpp"
#include "Dice.hpp"
#include "BankruptcyController.hpp"
#include <algorithm>

using namespace std;


UtilityTile::UtilityTile(int idx, string code, string name, int price, int morgageValue, vector<int> rentPrices) 
    : PropertyTile(idx, code, name, price, morgageValue, rentPrices) {
    this->PropertyType = UTILITY;
}

void UtilityTile::land(Player &p) {
    if (!isOwned()) {
        setOwner(&p);
        setStatus(OWNED);
    } else if (owner != &p && status == OWNED) {
        Player *ownerPlayer = owner;
        Dice dice;
        dice.roll();
        int diceRoll = dice.getTotal();
        ownerPlayer->countOwnerUtilities();
        int rent = diceRoll *  rentPrices[std::max(ownerPlayer->countOwnerUtilities() - 1, (int)(rentPrices.size() - 1))];
        if (!p.canAfford(rent)) {
            BankruptcyController::checkSolvency(p, rent);
        } 
        p -= rent;
        *ownerPlayer += rent;
    }
}

