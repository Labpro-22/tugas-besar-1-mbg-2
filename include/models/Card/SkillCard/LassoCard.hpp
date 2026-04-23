#pragma once
#include "SkillCard.hpp"

class LassoCard : public SkillCard {
public:
    LassoCard();
    ~LassoCard() override = default;
    SkillCardType getSkillType() const override;
};
