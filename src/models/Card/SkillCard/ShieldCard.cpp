#include "ShieldCard.hpp"

ShieldCard::ShieldCard() 
: SkillCard("ShieldCard", "Protect against rent charges/penalties for 1 turn.") {}

SkillCardType ShieldCard::getSkillType() const { 
    return SkillCardType::SHIELD;
}