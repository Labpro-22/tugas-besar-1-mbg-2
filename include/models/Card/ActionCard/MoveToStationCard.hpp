#pragma once
#include "ActionCard.hpp"
#include "GameBoard.hpp"

class MoveToStationCard : public ActionCard {
public:
    MoveToStationCard();
    ~MoveToStationCard() override = default;
    ActionCardType getActionType() const override;
};
