#include "NyalegCard.hpp"

NyalegCard::NyalegCard() 
    : ActionCard("NyalegCard", "You want to NYALEG. Pay M200 to each player for their vote.") {}

ActionCardType NyalegCard::getActionType() const {
    return ActionCardType::NYALEG;
}