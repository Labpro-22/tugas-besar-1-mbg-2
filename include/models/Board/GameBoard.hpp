#pragma once
#include <vector>
#include <string>
#include "PropertyTile.hpp"
#include "StreetTile.hpp"
#include "RailroadTile.hpp"

using namespace std;

class GameBoard
{
private:
    vector<Tile*> tiles;
    int totalTiles;
public:
    GameBoard();
    ~GameBoard() = default;
    void addTile(Tile* tile);

    void SortTilesByIndex();
    vector<Tile*> getTiles() const;
    vector<PropertyTile*> getPropertyTile() const;
    Tile* getTile(int position) const;
    Tile* getTileByCode(string code) const;
    Tile* getTileByName(string name) const;

    PropertyTile* getPropertyTileByCode(string code) const;
    int getTotalTile() const;
    int getIndexOfTile(Tile* tile) const;
    vector<PropertyTile*> getTileByColor(int position) const;
    map<string, vector<StreetTile*>> getMapColorProperty();
    int calculateTargetPosition(int currePos, int steps) const;
    int findNearestStation(int startPos) const;
};

