#pragma once

#pragma once
#include "Player.hpp"
#include "PropertyTile.hpp"
#include "DisplayView.hpp"
#include "StreetTile.hpp"
#include "EconomyController.hpp"
#include "GameContext.hpp"
#include "InputHandler.hpp"
#include <numeric>
#include <algorithm>
#include <vector>
#include <map>


enum class LiquidationType{
    SELL,
    MORTGAGE
};

class LiquidationTile{
    private:
        PropertyTile* tile;
        LiquidationType type;
    public:
        LiquidationTile() : tile(nullptr), type(LiquidationType::SELL) {}
        LiquidationTile(PropertyTile* tile, LiquidationType type) : tile(tile), type(type) {}
        PropertyTile* getTile() const { return tile; }
        LiquidationType getType() const { return type; }
};

class LiquidationOption {
    private:
        PropertyTile* tile;
        LiquidationType type;
        int value;
    public:
        LiquidationOption(PropertyTile* tile, LiquidationType type, int value) : tile(tile), type(type), value(value) {}
        PropertyTile* getTile() const { return tile; }
        LiquidationType getType() const { return type; }
        int getValue() const { return value; }
};

