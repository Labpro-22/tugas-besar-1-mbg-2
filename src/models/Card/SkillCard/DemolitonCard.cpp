#include "DemolitionCard.hpp"

DemolitionCard::DemolitionCard() 
: SkillCard("Demolition", "Demolish enemy's property.") {}

SkillCardType DemolitionCard::getSkillType() const { 
    return SkillCardType::DEMOLITION;
}