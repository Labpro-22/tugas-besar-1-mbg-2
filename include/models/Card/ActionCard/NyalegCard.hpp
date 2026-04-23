#pragma once
#include "ActionCard.hpp"

class NyalegCard : public ActionCard {
public:
    NyalegCard();
    ~NyalegCard() override = default;
    ActionCardType getActionType() const override;
};
