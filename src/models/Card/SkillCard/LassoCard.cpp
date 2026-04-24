#include "LassoCard.hpp"

LassoCard::LassoCard() 
: SkillCard("Lasso Card", "Pull enemy in front of you.") {}

SkillCardType LassoCard::getSkillType() const { 
    return SkillCardType::LASSO; 
}