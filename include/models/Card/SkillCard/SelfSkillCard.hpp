#pragma once
#include "SkillCard.hpp"
#include "Player.hpp"
#include "GameBoard.hpp"

class SelfSkillCard : public SkillCard
{
public:
    SelfSkillCard(string name, string desc);
    virtual void useSkill(Player* user, GameBoard* board) = 0;
    ~SelfSkillCard() override;
};