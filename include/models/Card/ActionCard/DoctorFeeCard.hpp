<<<<<<< HEAD
#pragma once
#include "ActionCard.hpp"

class DoctorFeeCard : public ActionCard {
public:
    DoctorFeeCard();
    ~DoctorFeeCard() override = default;
    ActionCardType getActionType() const override;
};
=======
#pragma once
#include "ActionCard.hpp"

class DoctorFeeCard : public ActionCard
{
private:
    int fee; 
public:
    DoctorFeeCard(string name, string desc, int fee);
    void execute(Player &p) override;
    ~DoctorFeeCard() override;
};
>>>>>>> 2242d01afd5f4da5b17612f349db6105653b5b58
