#include "TeleportCard.hpp"

TeleportCard::TeleportCard() 
: SkillCard("Teleport", "Move to any tile on board.") {}

SkillCardType TeleportCard::getSkillType() const { 
    return SkillCardType::TELEPORT; 
}