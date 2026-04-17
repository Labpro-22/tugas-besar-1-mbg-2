#pragma once
#include <string>
#include "Player.hpp"
using namespace std;

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
    string color = "White"; // default color, bisa diubah untuk property tile @suggestion by me
public:
    // virtual land(Player &p) = default; 
    // harusnya ga perlu pure virtual function karena tidak semua tile harus 
    // di-land, misal tile aksi atau special yang efeknya langsung aktif 
    // saat player melewati atau mendarat di tile tersebut
    Tile(int idx, string code, string name, TileType type);
    virtual ~Tile() = default; // virtual destructor

    int getIdx() const;
    string getCode() const;
    string getName() const;
    string getTotalTile() const;
    string getType() const;
};
