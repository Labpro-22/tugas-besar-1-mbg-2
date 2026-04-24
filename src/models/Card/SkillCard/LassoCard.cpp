#include "LassoCard.hpp"

LassoCard::LassoCard() 
: SkillCard("LassoCard", "Pull enemy in front of you.") {}

SkillCardType LassoCard::getSkillType() const { 
    return SkillCardType::LASSO; 
}