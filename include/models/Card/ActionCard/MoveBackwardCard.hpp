#pragma once
#include "ActionCard.hpp"

class MoveBackwardCard : public ActionCard
{
private:
    int steps;
public:
    MoveBackwardCard(string name, string desc, int steps);
    void execute(Player &p) override;
    ~MoveBackwardCard() override;
};
