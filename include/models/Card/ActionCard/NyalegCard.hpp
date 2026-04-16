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
