#pragma once
#include <list>
#include "Player.hpp"

using namespace std;

class TurnController
{
public:
    void nextTurn();
    
    void setMaxTurns(int maxTurns);
    void setStartingMoney(int startingMoney);
    
    int getCurrentTurn();
    bool checkWinCondition();
    void exportGameState(); 
    void executeAction();
};

