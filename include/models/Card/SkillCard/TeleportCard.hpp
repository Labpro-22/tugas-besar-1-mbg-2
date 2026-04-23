#pragma once
#include "SelfSkillCard.hpp"

class TeleportCard : public SelfSkillCard
{
private:
    int targetTileIndex;
public:
    TeleportCard(string name, string desc, int targetTileIndex);
    void setTargetLocation(int index);
    void useSkill(Player* user, GameBoard* board) override;
    ~TeleportCard() override;
};
