#include "DoctorFeeCard.hpp"

DoctorFeeCard::DoctorFeeCard() 
: ActionCard("Biaya Dokter", "Biaya dokter. Bayar M700.") {}

ActionCardType DoctorFeeCard::getActionType() const {
    return ActionCardType::DOCTOR_FEE;
}