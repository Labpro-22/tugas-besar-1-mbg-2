#include "TeleportCard.hpp"

TeleportCard::TeleportCard() 
    : SkillCard("Kartu Teleportasi", "Berpindah ke petak manapun di atas papan.") {}

SkillCardType TeleportCard::getSkillType() const { 
    return SkillCardType::TELEPORT; 
}