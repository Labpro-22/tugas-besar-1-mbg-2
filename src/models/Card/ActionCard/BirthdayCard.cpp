#include "BirthdayCard.hpp"

BirthdayCard::BirthdayCard() 
: ActionCard("Birthday", "It is your birthday. Collect M100 from every player.") {}

ActionCardType BirthdayCard::getActionType() const {
    return ActionCardType::BIRTHDAY;
}