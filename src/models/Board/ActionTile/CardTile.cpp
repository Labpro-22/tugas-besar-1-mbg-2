#include "CardTile.hpp"

CardTile::CardTile(int idx, string code, string name, CardType cardType, string color)
    : ActionTile(idx, code, name, CARD_TILE, color), cardType(cardType) {
}

LandResult CardTile::land(Player &p) {
    if (this->cardType == CardType::CHANCE) {
        return LandResult(LandEventType::DRAWCHANCE, this, nullptr, &p, nullptr, 0, false);
    }
    else {
        return LandResult(LandEventType::DRAWCOMMUNITYCHEST, this, nullptr, &p, nullptr, 0, false);
    }
};


