<<<<<<< HEAD
#pragma once
#include "ActionCard.hpp"

class BirthdayCard : public ActionCard {
public:
    BirthdayCard();
    ~BirthdayCard() override = default;
    ActionCardType getActionType() const override;
};
=======
#pragma once
#include "ActionCard.hpp"

class BirthdayCard : public ActionCard
{
private:
    vector<Player*> players; 
public:
    BirthdayCard(string name, string desc, vector<Player*> players);
    void execute(Player &p) override;
    ~BirthdayCard() override;
};
>>>>>>> 2242d01afd5f4da5b17612f349db6105653b5b58
