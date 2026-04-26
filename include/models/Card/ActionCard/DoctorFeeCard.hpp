#pragma once
#include "ActionCard.hpp"

class DoctorFeeCard : public ActionCard {
public:
    DoctorFeeCard();
    ~DoctorFeeCard() override = default;
    ActionCardType getActionType() const override;
};
