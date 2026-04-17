#pragma once
#include "Card.hpp"

class SkillCard : public Card
{
public:
    SkillCard(string name, string desc);
    ~SkillCard() override;
};

