#include "GameBoard.hpp"
#include <algorithm>
GameBoard::GameBoard() : totalTiles(0) {}

void GameBoard::addTile(Tile* tile)
{
    if (tile == nullptr)
    {
        return;
    }
    tiles.push_back(tile);
    totalTiles++;
}

Tile* GameBoard::getTile(int position) const
{
    if (position >= 0 && position < totalTiles)
    {
        return tiles[position];
    }
    return nullptr; 
}

Tile* GameBoard::getTileByCode(string code) const
{
    for (Tile* tile : tiles)
    {
        if (tile->getCode() == code)
        {
            return tile;
        }
    }
    return nullptr; 
}

Tile* GameBoard::getTileByName(string name) const
{
    for (Tile* tile : tiles)
    {
        if (tile->getName() == name)
        {
            return tile;
        }
    }
    return nullptr; 
}

int GameBoard::getTotalTile() const
{
    return totalTiles;
}

int GameBoard::getIndexOfTile(Tile* tile) const
{
    for (size_t i = 0; i < tiles.size(); ++i)
    {
        if (tiles[i] == tile)
        {
            return i;
        }
    }
    return -1; 
}

vector<PropertyTile*> GameBoard::getTileByColor(int position) const
{
    vector<PropertyTile*> result;
    if (position >= 0 && position < totalTiles)
    {
        Tile* tile = tiles[position];
        if (tile->getType() == "PROPERTY")
        {
            PropertyTile* propertyTile = dynamic_cast<PropertyTile*>(tile);
            if (propertyTile != nullptr)
            {
                string color = propertyTile->getColor();
                for (Tile* t : tiles)
                {
                    if (t->getType() == "PROPERTY")
                    {
                        PropertyTile* pt = dynamic_cast<PropertyTile*>(t);
                        if (pt != nullptr && pt->getColor() == color)
                        {
                            result.push_back(pt);
                        }
                    }
                }
            }
        }
    }
    return result;
}


map<string, vector<StreetTile*>> GameBoard::getMapColorProperty() {
    map<string, vector<StreetTile*>> colorMap;
    for (Tile* tile : tiles) {
        if (tile->getType() == "STREET") {
            StreetTile* streetTile = dynamic_cast<StreetTile*>(tile);
            if (streetTile != nullptr) {
                string color = streetTile->getColor();
                colorMap[color].push_back(streetTile);
            }
        }
    }
    return colorMap;
}

vector<PropertyTile*> GameBoard::getPropertyTile() const {
    vector<PropertyTile*> properties;
    for (Tile* tile : tiles) {
        if (tile->getType() == "PROPERTY") {
            PropertyTile* propertyTile = dynamic_cast<PropertyTile*>(tile);
            if (propertyTile != nullptr) {
                properties.push_back(propertyTile);
            }
        }
    }
    return properties;
}

void GameBoard::SortTilesByIndex() {
    sort(tiles.begin(), tiles.end(), [](Tile* a, Tile* b) {
        return a->getIdx() < b->getIdx();
    });
}
int GameBoard::calculateTargetPosition(int currPos, int steps) const {
    int boardSize = totalTiles;
    int targetPos = (currPos + steps) % totalTiles;

    if (targetPos < 0) {
        targetPos += totalTiles;
    }

    return targetPos;
}

int GameBoard::findNearestStation(int startPos) const {
    int searchPos = this->calculateTargetPosition(startPos, 1);

    while (true) {
        Tile* tileToCheck = this->getTile(searchPos);
        RailroadTile* rrTile = dynamic_cast<RailroadTile*>(tileToCheck);
        
        if (rrTile != nullptr) { 
            return searchPos;
        }
        searchPos = this->calculateTargetPosition(searchPos, 1);
    }
}

PropertyTile* GameBoard::getPropertyTileByCode(string code) const {
    for (Tile* tile : tiles) {
        if (tile->getType() == "PROPERTY") {
            PropertyTile* propertyTile = dynamic_cast<PropertyTile*>(tile);
            if (propertyTile != nullptr && propertyTile->getCode() == code) {
                return propertyTile;
            }
        }
    }
    return nullptr; 
}