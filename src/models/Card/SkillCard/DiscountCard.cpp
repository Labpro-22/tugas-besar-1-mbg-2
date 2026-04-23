#include "DiscountCard.hpp"
#include <random>

static mt19937& getRNG() {
    static random_device rd;
    static mt19937 gen(rd());
    return gen;
}

DiscountCard::DiscountCard() : SkillCard("Kartu Diskon", "Diskon sewa/sanksi."), duration(1) {
    uniform_int_distribution<> dist(1, 9);
    this->discountPercentage = dist(getRNG()) * 10; 
    
    this->description = "Memberikan diskon " + std::to_string(this->discountPercentage) + 
                        "% selama " + to_string(this->duration) + " giliran.";
}

SkillCardType DiscountCard::getSkillType() const { 
    return SkillCardType::DISCOUNT; 
}

int DiscountCard::getDiscountPercentage() const {
    return discountPercentage;
}

int DiscountCard::getDuration() const { 
    return duration; 
}