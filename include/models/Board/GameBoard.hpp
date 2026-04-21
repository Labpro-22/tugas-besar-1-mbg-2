#pragma once
#include <vector>
#include <string>
#include "PropertyTile.hpp"

using namespace std;

class GameBoard
{
private:
    vector<Tile*> tiles;
    int totalTiles;
public:
    ~GameBoard();
    void addTile(Tile* tile);

    Tile* getTile(int position) const;
    Tile* getTileByCode(string code) const;
    Tile* getTileByName(string name) const;
    int getTotalTile() const;
    int getIndexOfTile(Tile* tile) const;
    vector<PropertyTile*> getTileByColor(int position) const;
    
    void moveToPos(int currentPos, int steps);
};

