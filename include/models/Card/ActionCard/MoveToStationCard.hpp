#pragma once
#include "ActionCard.hpp"
#include "GameBoard.hpp"

class MoveToStationCard: public ActionCard
{
private:
    GameBoard* gameBoard; // pointer ke game board untuk memindahkan pemain
public:
    MoveToStationCard(string name, string desc, GameBoard* board);
    void execute(Player &p) override;
    ~MoveToStationCard() override;
};
