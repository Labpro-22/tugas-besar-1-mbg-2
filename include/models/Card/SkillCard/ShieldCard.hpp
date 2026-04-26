#pragma once
#include "SkillCard.hpp"

class ShieldCard : public SkillCard {
public:
    ShieldCard();
    ~ShieldCard() override = default;
    SkillCardType getSkillType() const override;
};
