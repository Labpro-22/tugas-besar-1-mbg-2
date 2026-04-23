#include "NyalegCard.hpp"

NyalegCard::NyalegCard() 
    : ActionCard("Nyaleg", "Anda mau nyaleg. Bayar M200 kepada setiap pemain.") {}

ActionCardType NyalegCard::getActionType() const {
    return ActionCardType::NYALEG;
}