#pragma once
#include "SkillCard.hpp"

class JailFreeCard : public SkillCard {
public:
    JailFreeCard();
    ~JailFreeCard() override = default;
    SkillCardType getSkillType() const override;
};
