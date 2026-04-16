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
