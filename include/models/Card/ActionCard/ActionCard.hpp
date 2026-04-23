<<<<<<< HEAD
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
=======
#pragma once
#include "Card.hpp"
#include "Player.hpp"

class ActionCard : public Card
{
public:
    ActionCard(string name, string desc);
    virtual void execute(Player &p) = 0; 
    ~ActionCard() override;
};

>>>>>>> 2242d01afd5f4da5b17612f349db6105653b5b58
