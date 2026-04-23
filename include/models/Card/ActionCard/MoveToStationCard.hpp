<<<<<<< HEAD
#pragma once
#include "ActionCard.hpp"
#include "GameBoard.hpp"

class MoveToStationCard : public ActionCard {
public:
    MoveToStationCard();
    ~MoveToStationCard() override = default;
    ActionCardType getActionType() const override;
};
=======
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
>>>>>>> 2242d01afd5f4da5b17612f349db6105653b5b58
