#pragma once
#include "ActionCard.hpp"

class MoveToJailCard : public ActionCard {
public:
    MoveToJailCard();
    ~MoveToJailCard() override = default;
    ActionCardType getActionType() const override;
};

