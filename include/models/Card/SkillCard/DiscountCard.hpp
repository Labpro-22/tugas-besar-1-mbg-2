#pragma once
#include "SkillCard.hpp"

class DiscountCard : public SkillCard {
private:
    int discountPercentage; 
public:
    DiscountCard();
    DiscountCard(int disc);
    ~DiscountCard() override = default;
    
    SkillCardType getSkillType() const override;
    int getDiscountPercentage() const;
    int getDuration() const;
};