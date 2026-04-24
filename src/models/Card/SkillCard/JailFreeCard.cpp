#include "JailFreeCard.hpp"

JailFreeCard::JailFreeCard() 
: SkillCard("Jail Free Card", "Congrats! The President released you from the prison by himself.") {}

SkillCardType JailFreeCard::getSkillType() const { 
    return SkillCardType::JAILFREE; 
}