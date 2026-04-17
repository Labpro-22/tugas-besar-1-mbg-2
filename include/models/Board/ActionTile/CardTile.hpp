#pragma once
#include "ActionTile.hpp"
#include "Card.hpp"
class CardTile : public ActionTile
{
private:
    CardType cardType;
    
public:
    CardTile(int idx, string code, string name, CardType cardType);
    void land(Player &p) override;
};

