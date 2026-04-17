#pragma once
#include "SelfSkillCard.hpp"

class ShieldCard : public SelfSkillCard
{
public:
    ShieldCard(string name, string desc);
    void useSkill(Player* user, GameBoard* board) override;
    ~ShieldCard() override;
};
