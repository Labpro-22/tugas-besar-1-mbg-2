#include "DemolitionCard.hpp"

DemolitionCard::DemolitionCard() 
: SkillCard("DemolitionCard", "Demolish enemy's property.") {}

SkillCardType DemolitionCard::getSkillType() const { 
    return SkillCardType::DEMOLITION;
}