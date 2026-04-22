#include "CardTile.hpp"

CardTile::CardTile(int idx, string code, string name, CardType cardType)
    : ActionTile(idx, code, name, CARD_TILE), cardType(cardType) {
}

LandResult CardTile::land(GameContext &G){
    return LandResult{LandEventType::DRAWCARD, this, nullptr, G.getCurrentPlayer(), nullptr, 0, false, string("You landed on a card tile. Draw a card from the deck.")};
};


