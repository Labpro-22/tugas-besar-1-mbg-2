#pragma once
#include "ActionTile.hpp"
#include "Card.hpp"

using namespace std;

class CardTile : public ActionTile
{
private:
    CardType cardType;
    
public:
    CardTile(int idx, string code, string name, CardType cardType);
    LandResult land(GameContext &G) override;
};

