#include "MoveBackwardCard.hpp"

MoveBackwardCard::MoveBackwardCard() 
: ActionCard("Mundur 3 Petak", "Mundur 3 petak.") {}

ActionCardType MoveBackwardCard::getActionType() const {
    return ActionCardType::MOVE_BACKWARD;
}