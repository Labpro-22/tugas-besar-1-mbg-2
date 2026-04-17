#pragma once
#include "Card.hpp"
#include "Player.hpp"

class ActionCard : public Card
{
public:
    ActionCard(string name, string desc);
    virtual void execute(Player &p) = 0; 
    ~ActionCard() override;
};

