#include "BirthdayCard.hpp"

BirthdayCard::BirthdayCard() 
: ActionCard("Ulang Tahun", "Ini adalah hari ulang tahun Anda. Dapatkan M100 dari setiap pemain.") {}

ActionCardType BirthdayCard::getActionType() const {
    return ActionCardType::BIRTHDAY;
}