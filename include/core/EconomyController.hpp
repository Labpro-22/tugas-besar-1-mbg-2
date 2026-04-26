#pragma once
#include "GameContext.hpp"
#include <map>
#include <vector>
#include "RailroadTile.hpp"
#include "UtilityTile.hpp"

using namespace std;
class EconomyController
{

private:
    int calculateStreetRent(GameContext *gameContext, StreetTile *tile);
    int calculateRailroadRent(GameContext *gameContext, RailroadTile *tile);
    int calculateUtilityRent(GameContext *gameContext, UtilityTile *tile, int diceTotal);
    int getMinBuildingsInColorGroup(string &colorGroup) ;
    int getMaxBuildingsInColorGroup(string &colorGroup) ;

public:
    EconomyController() = default;
    int calculateRent(GameContext *gameContext, PropertyTile *tile, int diceTotal);
    void processIncomeTax(GameContext *gameContext, Player &player, int choice);
    void processLuxuryTax(GameContext *gameContext, Player &player);
    void playerAddGoSalary(Player &player, GameContext *gameContext);
    void payRent(Player &payer, Player &receiver, PropertyTile *tile, int diceTotal);

    void purchaseProperty(Player &player, PropertyTile *tile);
    void acquirePropertyFree(Player &player, PropertyTile *tile);
    void mortgageProperty(Player &player, PropertyTile *tile);
    void redeemProperty(Player &player, PropertyTile *tile);    
    int sellBuilding(Player& player, StreetTile* tile);

    void buildHouse(GameContext *gameContext, Player &player, StreetTile *tile);
    void upgradeToHotel(GameContext *gameContext, Player &player, StreetTile *tile);
    int sellAllBuildingsInColorGroup(GameContext *gameContext, Player &player, const std::string &colorGroup);
    int calculateMaxLiquidation(Player &player) ;
    void transferAllAssets(Player &bankrupt, Player &creditor);
    void returnAllAssetsToBank(Player &bankrupt);
    void returnSkillCardsToDeck(Player &bankrupt, GameContext *gameContext);

    bool comparePlayerRank(Player &a,  Player &b) ;

    // Helpers
    bool isMonopoly(GameContext *gameContext, StreetTile *tile) ;
    int countRailroadsOwned( Player &player) ;
    int countUtilitiesOwned( Player &player) ;
    vector<StreetTile *> getColorGroupTiles(GameContext *gameContext, const std::string &colorGroup) ;
    bool canBuildOnTile(GameContext *gameContext, StreetTile *tile) ;
    bool canUpgradeToHotel(GameContext *gameContext, const std::string &colorGroup) ;

};
