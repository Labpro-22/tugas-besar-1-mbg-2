#pragma once
#include "PropertyTile.hpp"
#include "Card.hpp"
#include "Player.hpp"
#include <map>
#include "CardDeck.hpp"

using namespace std;

class EffectController
{
private:
    map<PropertyTile*, int> festivalModifiers; 
    map<PropertyTile*, int> festivalDurations;
    CardDeck<Card> specialCards; // bingung type apa yang harusnya disini, 
public:
    void applyFestival(PropertyTile* tile);
    void decrementDurations();
    void executeCard(Player* player, CardType card);
};

