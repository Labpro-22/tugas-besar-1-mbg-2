#pragma once
#include "GameBoard.hpp"
#include "ConfigReader.hpp"
#include "SaveLoader.hpp"
#include "TurnController.hpp"
#include <string>

using namespace std;
class GameEngine
{
private:
    GameBoard gameBoard;
    ConfigReader configReader;
    SaveLoader saveLoader;
    TurnController turnController;
public:
    void initGame();
    void run();
    void processCommand(string cmd);

};

