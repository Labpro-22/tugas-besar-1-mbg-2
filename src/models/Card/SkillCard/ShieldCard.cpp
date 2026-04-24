#include "ShieldCard.hpp"

ShieldCard::ShieldCard() 
: SkillCard("Shield Card", "Protect against rent charges/penalties for 1 turn.") {}

SkillCardType ShieldCard::getSkillType() const { 
    return SkillCardType::SHIELD;
}