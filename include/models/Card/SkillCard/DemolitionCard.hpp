#pragma once
#include "SkillCard.hpp"

class DemolitionCard : public SkillCard {
public:
    DemolitionCard();
    ~DemolitionCard() override = default;
    SkillCardType getSkillType() const override;
};
