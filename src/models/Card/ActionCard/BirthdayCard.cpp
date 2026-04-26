#include "BirthdayCard.hpp"

BirthdayCard::BirthdayCard() 
: ActionCard("BirthdayCard", "It is your birthday. Collect M100 from every player.") {}

ActionCardType BirthdayCard::getActionType() const {
    return ActionCardType::BIRTHDAY;
}