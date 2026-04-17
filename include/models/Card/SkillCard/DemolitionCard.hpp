#pragma once
#include "AttackSkillCard.hpp"

class DemolitionCard : public AttackSkillCard
{
private:
    int propertyIndex;
public:
    DemolitionCard(string name, string desc);
    void setTargetProperty(int index);
    void useSkill(Player* user, Player* target, GameBoard* board) override;
    ~DemolitionCard() override;
};
