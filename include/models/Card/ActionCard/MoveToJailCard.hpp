<<<<<<< HEAD
#pragma once
#include "ActionCard.hpp"

class MoveToJailCard : public ActionCard {
public:
    MoveToJailCard();
    ~MoveToJailCard() override = default;
    ActionCardType getActionType() const override;
};

=======
#pragma once
#include "ActionCard.hpp"

class MoveToJailCard : public ActionCard
{
public:
    MoveToJailCard(string name, string desc);
    void execute(Player &p) override;
    ~MoveToJailCard() override;
};

>>>>>>> 2242d01afd5f4da5b17612f349db6105653b5b58
