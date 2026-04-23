#pragma once
#include <string>
#include "Player.hpp"
#include "LandResult.hpp"

using namespace std;

// Forward declaration
class GameContext;

enum TileType
{
    PROPERTY,
    AKSI, 
    SPECIAL,
};

class Tile
{
protected:
    int idx;
    string code;
    string name;
    TileType type; 
    string color = "DEFAULT"; // default color, bisa diubah untuk property tile @suggestion by me
public:
    virtual LandResult land(GameContext &G) = 0; // pure virtual function
    Tile(int idx, string code, string name, TileType type, string color);
    virtual ~Tile() = default; // virtual destructor

    int getIdx() const;
    string getCode() const;
    string getName() const;
    string getType() const;
    string getColor() const;
};
