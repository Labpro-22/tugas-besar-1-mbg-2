#include "Tile.hpp"
#include <iostream>


Tile::Tile(int idx, string code, string name, TileType type) : idx(idx), code(code), name(name), type(type) {}

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

// int main() {
//     // Example usage
//     Tile tile1(1, "P1", "Start", SPECIAL);
//     Tile tile2(2, "P2", "Park Place", PROPERTY);

//     cout << "Tile 1: " << tile1.getName() << ", Type: " << tile1.getType() << endl;
//     cout << "Tile 2: " << tile2.getName() << ", Type: " << tile2.getType() << endl;

//     return 0;
// }