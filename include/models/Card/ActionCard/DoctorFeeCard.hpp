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
