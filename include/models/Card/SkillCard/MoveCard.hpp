#pragma once
#include "SelfSkillCard.hpp"

class MoveCard : public SelfSkillCard
{
private:
    int steps;
public:
    MoveCard(string name, string desc, int steps);
    void useSkill(Player* user, GameBoard* board) override;
    ~MoveCard() override;
};
