#include "MoveToJailCard.hpp"

MoveToJailCard::MoveToJailCard() 
: ActionCard("Masuk Penjara", "Masuk Penjara.") {}

ActionCardType MoveToJailCard::getActionType() const {
    return ActionCardType::MOVE_TO_JAIL;
}