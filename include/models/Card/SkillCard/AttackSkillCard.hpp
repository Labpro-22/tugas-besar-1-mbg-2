#pragma once
#include "SkillCard.hpp"
#include "Player.hpp"
#include "GameBoard.hpp"

class AttackSkillCard : public SkillCard
{
public:
    AttackSkillCard(string name, string desc);
    virtual void useSkill(Player* user, Player* target, GameBoard* board) = 0;
    ~AttackSkillCard();
};

