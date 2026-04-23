#pragma once
#include "AttackSkillCard.hpp"

class LassoCard : public AttackSkillCard
{

public:
    LassoCard(string name, string desc);
    void useSkill(Player* user, Player* target, GameBoard* board) override;
    ~LassoCard() override;
};

