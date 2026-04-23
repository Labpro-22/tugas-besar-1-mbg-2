#include "DemolitionCard.hpp"

DemolitionCard::DemolitionCard() 
: SkillCard("Kartu Demolition", "Menghancurkan satu properti lawan.") {}

SkillCardType DemolitionCard::getSkillType() const { 
    return SkillCardType::DEMOLITION;
}