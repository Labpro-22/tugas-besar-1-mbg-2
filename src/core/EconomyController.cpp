#include "EconomyController.hpp"
#include "GameException.hpp"

#include <algorithm>
#include <iostream>

using namespace std;



int EconomyController::calculateRent(GameContext *gameContext, PropertyTile *tile, int diceTotal) {
    if (tile == nullptr) {
        return 0;
    }
    if (auto *streetTile = dynamic_cast<StreetTile *>(tile)) {
        return calculateStreetRent(gameContext, streetTile);
    }
    if (auto *railroadTile = dynamic_cast<RailroadTile *>(tile)) {
        return calculateRailroadRent(gameContext, railroadTile);
    }
    if (auto *utilityTile = dynamic_cast<UtilityTile *>(tile)) {
        return calculateUtilityRent(gameContext, utilityTile, diceTotal);
    }
    return 0;
}

void EconomyController::processIncomeTax(GameContext *gameContext, Player &player, int choice) {
    if (gameContext == nullptr) {
        return;
    }
    if (choice == 1) {
        player -= gameContext->getPphFlat();
        return;
    }
    if (choice == 2) {
        int amount = (player.getBalance() * gameContext->getPphPercentage()) / 100;
        player -= amount;
        return;
    }
}

void EconomyController::processLuxuryTax(GameContext *gameContext, Player &player) {
    if (gameContext == nullptr) {
        return;
    }
    player -= gameContext->getPbm();
}

void playerAddGoSalary(Player &player, GameContext *gameContext){
    player += gameContext->getGoSalary();
}

void EconomyController::purchaseProperty(Player &player, PropertyTile *tile) {
    if (tile == nullptr || tile->isOwned()) {
        throw AuctionTriggerException();
    }

    int price = tile->getPrice();
    if (!player.canAfford(price)) {
        throw InsufficientFundsException(price, player.getBalance());
    }

    player -= price;
    tile->setOwner(&player);
    tile->setStatus(OWNED);
    
    player.addProperty(tile);
}

void EconomyController::acquirePropertyFree(Player &player, PropertyTile *tile) {
    if (tile == nullptr || tile->isOwned()) {
        return;
    }
    tile->setOwner(&player);
    tile->setStatus(OWNED);
    player.addProperty(tile);
}

void EconomyController::mortgageProperty(Player &player, PropertyTile *tile) {
    if (tile == nullptr || tile->getOwner() != &player || tile->getStatus() == MORTGAGED) {
        return;
    }
    player += tile->getMortgageValue();
    tile->setStatus(MORTGAGED);
}

void EconomyController::redeemProperty(Player &player, PropertyTile *tile) {
    if (tile == nullptr || tile->getOwner() != &player || tile->getStatus() != MORTGAGED) {
        return;
    }

    int redeemCost = (tile->getPrice());
    if (!player.canAfford(redeemCost)) {
        throw InsufficientFundsException(redeemCost, player.getBalance());
    }
    player -= redeemCost;
    tile->setStatus(OWNED);
}


void EconomyController::buildHouse(GameContext *gameContext, Player &player, StreetTile *tile) {
    if (tile == nullptr || tile->getOwner() != &player || !canBuildOnTile(gameContext, tile)) {
        return;
    }

    const int cost = tile->getHouseCost();
    if (!player.canAfford(cost)) {
        throw InsufficientFundsException(cost, player.getBalance());
    }
    player -= cost;
    tile->setHouseCount(tile->getHouseCount() + 1);
}

void EconomyController::upgradeToHotel(GameContext *gameContext, Player &player, StreetTile *tile) {
    if (tile == nullptr || tile->getOwner() != &player || !canUpgradeToHotel(gameContext, tile->getColor())) {
        return;
    }

    if (tile->getHasHotel() || tile->getHouseCount() < 4) {
        return;
    }
    int cost = tile->getHotelCost();
    if (!player.canAfford(cost)) {
        throw InsufficientFundsException(cost, player.getBalance());
    }
    player -= cost;
    tile->setHasHotel(true);
}
int EconomyController::sellBuilding(Player& player, StreetTile* tile){
    if (!tile || tile->getOwner() != &player) return 0;

    int gain = 0;

    if (tile->getHasHotel()){
        gain = tile->getHotelCost()/2;
        tile->setHasHotel(false);
    }
    else if (tile->getHouseCount() > 0){
        gain = tile->getHouseCost()/2;
        tile->setHouseCount(tile->getHouseCount() - 1);
    }

    player += gain;
    return gain;
}

int EconomyController::sellAllBuildingsInColorGroup(GameContext *gameContext, Player &player, const std::string &colorGroup) {
    int total = 0;
    for (StreetTile *tile : getColorGroupTiles(gameContext, colorGroup)) {
        if (tile == nullptr || tile->getOwner() != &player) {
            continue;
        }

        total += tile->getBuildingValue() / 2;
        tile->setHouseCount(0);
        tile->setHasHotel(false);
    }

    if (total > 0) {
        player += total;
    }

    return total;
}


int EconomyController::calculateMaxLiquidation(Player& player) {
    int total = player.getBalance();

    for (PropertyTile* property : player.getOwnedProperties()) {
        if (property == nullptr) continue;

        if (property->getStatus() == MORTGAGED) {
            continue;
        }
        total += property->getPrice();

        if (auto* street = dynamic_cast<StreetTile*>(property)) {
            total += street->getBuildingValue() / 2;
        }
    }
    return total;
}

void EconomyController::transferAllAssets(Player &bankrupt, Player &creditor) {
    vector<PropertyTile *> properties = bankrupt.getOwnedProperties();
    for (PropertyTile *property : properties) {
        if (property == nullptr) {
            continue;
        }
        bankrupt.removeProperty(property);
        creditor.addProperty(property);
        property->setOwner(&creditor);
        property->setStatus(OWNED);
    }
}

void EconomyController::returnAllAssetsToBank(Player &bankrupt) {
    vector<PropertyTile *> properties = bankrupt.getOwnedProperties();
    for (PropertyTile *property : properties) {
        if (property == nullptr) {
            continue;
        }

        if (auto *streetTile = dynamic_cast<StreetTile *>(property)) {
            streetTile->setHouseCount(0);
            streetTile->setHasHotel(false);
        }

        bankrupt.removeProperty(property);
        property->setOwner(nullptr);
        property->setStatus(BANK);
    }

    
}

void EconomyController::returnSkillCardsToDeck(Player &bankrupt, GameContext *gameContext){ 
    int totalCards = bankrupt.getSkillCardCount();
    if (totalCards > 0) {
        cout << "- Mengembalikan " << totalCards << " Kartu Kemampuan ke tumpukan..." << endl;
        
        // Selalu ambil dari index 0 sampai tangan kosong
        while (bankrupt.getSkillCardCount() > 0) {
            SkillCard* returnedCard = bankrupt.dropSkillCard(0);
            gameContext->getSkillDeck().discard(returnedCard);
        }
    }
}

bool EconomyController::comparePlayerRank( Player &a, Player &b)  {
    const int wealthA = a.totalWealth();
    const int wealthB = b.totalWealth();

    if (wealthA != wealthB) {
        return wealthA > wealthB;
    }
    return a.getBalance() > b.getBalance();
}

bool EconomyController::isMonopoly(GameContext *gameContext, StreetTile *tile) {
    if (tile == nullptr || tile->getOwner() == nullptr) {
        return false;
    }

    vector<StreetTile *> colorGroupTiles = getColorGroupTiles(gameContext, tile->getColor());
    if (colorGroupTiles.empty()) {
        return false;
    }

    Player *owner = tile->getOwner();
    for (StreetTile *groupTile : colorGroupTiles) {
        if (groupTile == nullptr || groupTile->getOwner() != owner || groupTile->getStatus() == MORTGAGED) {
            return false;
        }
    }

    return true;
}

int EconomyController::countRailroadsOwned(Player &player)  {
    int count = 0;
    for (PropertyTile *property : player.getOwnedProperties()) {
        if (property != nullptr && property->getPropertyType() == RAILROAD) {
            count++;
        }
    }
    return count;
}

int EconomyController::countUtilitiesOwned(Player &player)  {
    int count = 0;
    for (PropertyTile *property : player.getOwnedProperties()) {
        if (property != nullptr && property->getPropertyType() == UTILITY) {
            count++;
        }
    }
    return count;
}

vector<StreetTile *> EconomyController::getColorGroupTiles(GameContext *gameContext, const std::string &colorGroup)  {
    vector<StreetTile *> tiles;
    if (gameContext == nullptr) {
        return tiles;
    }

    const int totalTiles = gameContext->getBoard().getTotalTile();

    for (int index = 0; index < totalTiles; ++index) {
        Tile *boardTile = gameContext->getBoard().getTile(index);
        auto *propertyTile = dynamic_cast<PropertyTile *>(boardTile);
        if (propertyTile == nullptr || propertyTile->getColor() != colorGroup || propertyTile->getPropertyType() != STREET) {
            continue;
        }

        auto *streetTile = dynamic_cast<StreetTile *>(propertyTile);
        if (streetTile != nullptr) {
            tiles.push_back(streetTile);
        }
    }

    return tiles;
}

bool EconomyController::canBuildOnTile(GameContext *gameContext, StreetTile *tile)  {
    if (tile == nullptr || tile->getOwner() == nullptr || tile->getStatus() != OWNED) return false;
    if (tile->getHasHotel()) return false;

    if (!isMonopoly(gameContext, tile)) return false;

    if (tile->getHouseCount() >= 4) return false;

    string colorGroup = tile->getColor();
    int minHouse = getMinBuildingsInColorGroup(gameContext, colorGroup);
    
    if (tile->getHouseCount() > minHouse) {
        return false;
    }

    return true;
}

int EconomyController::getMinBuildingsInColorGroup(GameContext *ctx, string &colorGroup) {
    vector<StreetTile*> tiles = getColorGroupTiles(ctx, colorGroup);
    if (tiles.empty()) return 0;

    int minHouse = tiles[0]->getHouseCount();
    for (StreetTile* tile : tiles) {
        if (tile != nullptr) {
            minHouse = min(minHouse, tile->getHouseCount());
        }
    }
    return minHouse;
}

bool EconomyController::canUpgradeToHotel(GameContext *gameContext, const std::string &colorGroup)  {
    vector<StreetTile *> tiles = getColorGroupTiles(gameContext, colorGroup);
    if (tiles.empty()) {
        return false;
    }

    Player *owner = tiles.front()->getOwner();
    if (owner == nullptr) {
        return false;
    }

    for (StreetTile *tile : tiles) {
        if (tile == nullptr || tile->getOwner() != owner || tile->getStatus() != OWNED) {
            return false;
        }

        if (tile->getHouseCount() < 4 || tile->getHasHotel()) {
            return false;
        }
    }

    return true;
}

int EconomyController::calculateStreetRent(GameContext *gameContext, StreetTile *tile) {
    if (tile == nullptr) {
        return 0;
    }

    const vector<int> rentPrices = tile->getRentPrices();
    if (rentPrices.empty()) {
        return 0;
    }

    int rentIndex = 0;
    if (tile->getHasHotel()) {
        rentIndex = min<int>(static_cast<int>(rentPrices.size()) - 1, 5);
    } else {
        rentIndex = min<int>(tile->getHouseCount(), static_cast<int>(rentPrices.size()) - 1);
        if (tile->getHouseCount() == 0 && isMonopoly(gameContext, tile)) {
            return rentPrices.front() * 2;
        }
    }

    int rent = rentPrices[rentIndex];
    if (tile->isFestivalActive()) {
        rent *= tile->getFestivalStack();
    }

    return rent;
}

int EconomyController::calculateRailroadRent(GameContext *gameContext, RailroadTile *tile) {
    if (gameContext == nullptr || tile == nullptr || tile->getOwner() == nullptr) {
        return 0;
    }

    const int ownedCount = countRailroadsOwned(*tile->getOwner());
    const auto &rentTable = gameContext->getRailroadRentTable();
    auto it = rentTable.find(ownedCount);
    if (it == rentTable.end()) {
        return 0;
    }

    return it->second;
}

int EconomyController::calculateUtilityRent(GameContext *gameContext, UtilityTile *tile, int diceTotal) {
    if (gameContext == nullptr || tile == nullptr || tile->getOwner() == nullptr) {
        return 0;
    }

    const int ownedCount = countUtilitiesOwned(*tile->getOwner());
    const auto &multiplierTable = gameContext->getUtilityMultiplierTable();
    auto it = multiplierTable.find(ownedCount);
    if (it == multiplierTable.end()) {
        return 0;
    }

    return diceTotal * it->second;
}

void EconomyController::payRent(Player &payer, Player &receiver, PropertyTile *tile, int diceTotal) {
    if (tile == nullptr || tile->getOwner() == nullptr || tile->getOwner() != &receiver) {
        return;
    }

    int rentAmount = calculateRent(nullptr, tile, diceTotal);
    if (rentAmount <= 0) {
        return;
    }

    if (!payer.canAfford(rentAmount)) {
        throw InsufficientFundsException(rentAmount, payer.getBalance());
    }

    payer -= rentAmount;
    receiver += rentAmount;
}

map<string, vector<StreetTile*>> EconomyController::buildableStreet(map<string, vector<StreetTile*>> colorGroupMap, GameContext *gameContext, Player &player) {
    map<string, vector<StreetTile*>> buildableGroups;
    map<string, vector<StreetTile*>> properties = gameContext->getBoard().getMapColorProperty();

    for (const auto& entry : colorGroupMap) {
        const vector<StreetTile*>& streets = entry.second;

        if (properties[entry.first].size() != streets.size()) {
            continue; // Skip color groups that are not fully owned
        }

        buildableGroups[entry.first] = streets;
    }

    return buildableGroups;
}

