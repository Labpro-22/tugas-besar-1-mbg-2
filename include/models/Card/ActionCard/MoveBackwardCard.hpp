#pragma once
#include "ActionCard.hpp"

class MoveBackwardCard : public ActionCard {
public:
    MoveBackwardCard();
    ~MoveBackwardCard() override = default;
    ActionCardType getActionType() const override;
};
