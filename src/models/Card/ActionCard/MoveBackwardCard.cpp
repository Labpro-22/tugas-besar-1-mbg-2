#include "MoveBackwardCard.hpp"

MoveBackwardCard::MoveBackwardCard() 
: ActionCard("MoveBackwardCard", "Move back 3 spaces.") {}

ActionCardType MoveBackwardCard::getActionType() const {
    return ActionCardType::MOVE_BACKWARD;
}