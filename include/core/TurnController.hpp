#pragma once
#include <list>
#include "Player.hpp"

using namespace std;

class TurnController
{
private:
    list<Player> players;
    int currentPlayerIndex;
    int currentTurn;
    int maxTurns;
public:
    void nextTurn();
    void executeAction(Action a);
    int getCurrentTurn();
    bool checkWinCondition();
    void exportGameState(); 
};

