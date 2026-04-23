#include "LassoCard.hpp"

LassoCard::LassoCard() 
: SkillCard("Kartu Lasso", "Menarik satu pemain lawan di depan ke petakmu.") {}

SkillCardType LassoCard::getSkillType() const { 
    return SkillCardType::LASSO; 
}