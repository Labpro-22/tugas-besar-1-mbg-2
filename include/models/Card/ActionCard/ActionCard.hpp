#pragma once
#include "Card.hpp"
#include "Player.hpp"

enum class ActionCardType {
    MOVE_TO_STATION,
    MOVE_BACKWARD,
    MOVE_TO_JAIL,
    BIRTHDAY,
    DOCTOR_FEE,
    NYALEG
};

class ActionCard : public Card
{
public:
    ActionCard(string name, string desc);
    ~ActionCard() override = default;

    virtual ActionCardType getActionType() const = 0;
};