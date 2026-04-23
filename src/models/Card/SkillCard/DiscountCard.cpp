#include "DiscountCard.hpp"
#include <random>

static mt19937& getRNG() {
    static random_device rd;
    static mt19937 gen(rd());
    return gen;
}

DiscountCard::DiscountCard() : SkillCard("Discount", "Get a random discount.") {
    uniform_int_distribution<> dist(1, 9);
    this->discountPercentage = dist(getRNG()) * 10; 
    
    this->description = "Get a " + to_string(this->discountPercentage) + "% discount for 1 turn.";
}

SkillCardType DiscountCard::getSkillType() const { 
    return SkillCardType::DISCOUNT; 
}

int DiscountCard::getDiscountPercentage() const {
    return discountPercentage;
}

int DiscountCard::getDuration() const { 
    return 1; 
}