#pragma once
#include "SkillCard.hpp"

class TeleportCard : public SkillCard {
public:
    TeleportCard();
    ~TeleportCard() override = default;
    SkillCardType getSkillType() const override;
};