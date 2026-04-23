#pragma once
#include "PropertyTile.hpp"
#include "Card.hpp"
#include "Player.hpp"
#include <map>
#include "CardDeck.hpp"

using namespace std;

class EffectController
{
public:
    void applyFestival(PropertyTile* tile);
    void decrementDurations();
    void executeCard(Player* player, CardType card);
};

