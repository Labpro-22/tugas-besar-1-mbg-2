#include "CardTile.hpp"

CardTile::CardTile(int idx, string code, string name, CardType cardType, string color)
    : ActionTile(idx, code, name, CARD_TILE, color), cardType(cardType) {
}

LandResult CardTile::land(GameContext &G){
    return LandResult{LandEventType::DRAWCARD, this, nullptr, &G.getCurrentPlayer(), nullptr, 0, false};
};


