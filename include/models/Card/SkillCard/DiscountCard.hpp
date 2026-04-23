#pragma once
#include "SkillCard.hpp"

class DiscountCard : public SkillCard {
private:
    int discountPercentage; 
    int duration;
public:
    DiscountCard();
    ~DiscountCard() override = default;
    
    SkillCardType getSkillType() const override;
    int getDiscountPercentage() const;
    int getDuration() const;
};

