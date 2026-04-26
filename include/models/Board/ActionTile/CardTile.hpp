#pragma once
#include "ActionTile.hpp"
#include "Card.hpp"
#include "GameContext.hpp"

using namespace std;

enum CardType {
    CHANCE,
    COMMUNITY_CHEST
};

class CardTile : public ActionTile
{
private:
    CardType cardType;
    
public:
    CardTile(int idx, string code, string name, CardType cardType, string color = "DEFAULT");
    LandResult land(GameContext &G) override;
};

