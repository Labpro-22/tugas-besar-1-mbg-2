#include "MoveToJailCard.hpp"

MoveToJailCard::MoveToJailCard() 
: ActionCard("MoveToJailCard", "Go directly to Jail.") {}

ActionCardType MoveToJailCard::getActionType() const {
    return ActionCardType::MOVE_TO_JAIL;
}