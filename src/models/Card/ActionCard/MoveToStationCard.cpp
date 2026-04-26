#include "MoveToStationCard.hpp"

MoveToStationCard::MoveToStationCard() 
: ActionCard("MoveToStationCard", "Go to the nearest station.") {}

ActionCardType MoveToStationCard::getActionType() const {
    return ActionCardType::MOVE_TO_STATION;
}

