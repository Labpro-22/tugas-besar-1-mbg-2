#include "Tile.hpp"
#include <iostream>


Tile::Tile(int idx, string code, string name, TileType type, string color) 
: idx(idx), code(code), name(name), type(type), color(color) {}

int Tile::getIdx() const {
    return idx;
}

string Tile::getCode() const {
    return code;
}

string Tile::getName() const {
    return name;
}

string Tile::getType() const {
    switch (type) {
        case PROPERTY:
            return "PROPERTY";
        case AKSI:
            return "AKSI";
        case SPECIAL:
            return "SPECIAL";
        default:
            return "UNKNOWN";
    }
}

string Tile::getColor() const {
    return color;
}
