#include "MoveToStationCard.hpp"

MoveToStationCard::MoveToStationCard() 
: ActionCard("Move To Station", "Go to the nearest station.") {}

ActionCardType MoveToStationCard::getActionType() const {
    return ActionCardType::MOVE_TO_STATION;
}

