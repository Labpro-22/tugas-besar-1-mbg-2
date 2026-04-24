#include "MoveBackwardCard.hpp"

MoveBackwardCard::MoveBackwardCard() 
: ActionCard("Move Backward Card", "Move back 3 spaces.") {}

ActionCardType MoveBackwardCard::getActionType() const {
    return ActionCardType::MOVE_BACKWARD;
}