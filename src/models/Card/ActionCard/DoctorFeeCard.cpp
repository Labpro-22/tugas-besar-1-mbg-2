#include "DoctorFeeCard.hpp"

DoctorFeeCard::DoctorFeeCard() 
: ActionCard("Doctor Fee Card", "Doctor's fee. Pay M700.") {}

ActionCardType DoctorFeeCard::getActionType() const {
    return ActionCardType::DOCTOR_FEE;
}