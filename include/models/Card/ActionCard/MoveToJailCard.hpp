#pragma once
#include "ActionCard.hpp"

class MoveToJailCard : public ActionCard
{
public:
    MoveToJailCard(string name, string desc);
    void execute(Player &p) override;
    ~MoveToJailCard() override;
};

