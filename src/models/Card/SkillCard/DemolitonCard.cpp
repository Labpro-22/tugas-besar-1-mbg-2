#include "DemolitionCard.hpp"

DemolitionCard::DemolitionCard() 
: SkillCard("Demolition Card", "Demolish enemy's property.") {}

SkillCardType DemolitionCard::getSkillType() const { 
    return SkillCardType::DEMOLITION;
}