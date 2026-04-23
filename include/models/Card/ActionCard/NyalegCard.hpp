<<<<<<< HEAD
#pragma once
#include "ActionCard.hpp"

class NyalegCard : public ActionCard {
public:
    NyalegCard();
    ~NyalegCard() override = default;
    ActionCardType getActionType() const override;
};
=======
#pragma once
#include "ActionCard.hpp"

class NyalegCard : public ActionCard    
{
private:
    vector <Player*> allPlayers;
    int amount;
public:
    NyalegCard(string name, string desc, vector <Player*> players);
    void execute(Player &p) override;
    ~NyalegCard() override;
};
>>>>>>> 2242d01afd5f4da5b17612f349db6105653b5b58
