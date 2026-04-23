#pragma once
#include "ActionCard.hpp"

class BirthdayCard : public ActionCard {
public:
    BirthdayCard();
    ~BirthdayCard() override = default;
    ActionCardType getActionType() const override;
};
