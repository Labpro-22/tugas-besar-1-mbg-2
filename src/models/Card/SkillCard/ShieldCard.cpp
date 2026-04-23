#include "ShieldCard.hpp"

ShieldCard::ShieldCard() 
: SkillCard("Kartu Perisai", "Melindungi dari tagihan sewa/sanksi selama 1 giliran.") {}

SkillCardType ShieldCard::getSkillType() const { 
    return SkillCardType::SHIELD;
}