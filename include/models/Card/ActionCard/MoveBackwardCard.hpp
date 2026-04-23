<<<<<<< HEAD
#pragma once
#include "ActionCard.hpp"

class MoveBackwardCard : public ActionCard {
public:
    MoveBackwardCard();
    ~MoveBackwardCard() override = default;
    ActionCardType getActionType() const override;
};
=======
#pragma once
#include "ActionCard.hpp"

class MoveBackwardCard : public ActionCard
{
private:
    int steps;
public:
    MoveBackwardCard(string name, string desc, int steps);
    void execute(Player &p) override;
    ~MoveBackwardCard() override;
};
>>>>>>> 2242d01afd5f4da5b17612f349db6105653b5b58
