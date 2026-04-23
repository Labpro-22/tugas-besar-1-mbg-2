#pragma once
#include "SelfSkillCard.hpp"

class DiscountCard : public SelfSkillCard
{
private:
    int discountPercentage;
public:
    DiscountCard(string name, string desc, int discountPercentage);
    void useSkill(Player* user, GameBoard* board) override;
    ~DiscountCard() override;
};

