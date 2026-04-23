#include "MoveToJailCard.hpp"

MoveToJailCard::MoveToJailCard() 
: ActionCard("Move To Jail", "Go directly to Jail.") {}

ActionCardType MoveToJailCard::getActionType() const {
    return ActionCardType::MOVE_TO_JAIL;
}