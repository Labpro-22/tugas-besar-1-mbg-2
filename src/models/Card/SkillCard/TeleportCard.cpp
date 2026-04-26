#include "TeleportCard.hpp"

TeleportCard::TeleportCard() 
: SkillCard("TeleportCard", "Move to any tile on board.") {}

SkillCardType TeleportCard::getSkillType() const { 
    return SkillCardType::TELEPORT; 
}