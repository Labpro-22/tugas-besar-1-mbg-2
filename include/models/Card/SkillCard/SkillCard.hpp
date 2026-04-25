#pragma once
#include "Card.hpp"

enum class SkillCardType {
    MOVE,
    DISCOUNT,
    SHIELD,
    TELEPORT,
    LASSO,
    DEMOLITION,
    JAILFREE
};

class SkillCard : public Card
{
public:
    SkillCard(string name, string desc);
    ~SkillCard() override = default;

    virtual SkillCardType getSkillType() const = 0;
};

