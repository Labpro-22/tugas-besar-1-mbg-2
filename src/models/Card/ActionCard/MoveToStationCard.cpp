#include "MoveToStationCard.hpp"

MoveToStationCard::MoveToStationCard() 
: ActionCard("Pergi ke Stasiun", "Pergi ke stasiun terdekat.") {}

ActionCardType MoveToStationCard::getActionType() const {
    return ActionCardType::MOVE_TO_STATION;
}

