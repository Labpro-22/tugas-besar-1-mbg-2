#pragma once
#include "SkillCard.hpp"

class MoveCard : public SkillCard {
private:
    int steps;
public:
    MoveCard();
    MoveCard(int steps);
    ~MoveCard() override = default;
    
    SkillCardType getSkillType() const override;
    int getSteps() const;
};
