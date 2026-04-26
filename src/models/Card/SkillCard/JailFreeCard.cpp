#include "JailFreeCard.hpp"

JailFreeCard::JailFreeCard() 
: SkillCard("JailFreeCard", "Congrats! The President released you from the prison by himself.") {}

SkillCardType JailFreeCard::getSkillType() const { 
    return SkillCardType::JAILFREE; 
}