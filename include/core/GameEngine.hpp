#pragma once
#include "GameBoard.hpp"
#include "../utils/ConfigReader.hpp"
#include "GameContext.hpp"
#include "SaveLoader.hpp"
#include "TurnController.hpp"
#include <string>
#include "AllCards.hpp"

using namespace std;
class GameEngine
{
public:
    void initGame(GameContext &gameContext,
                  TurnController &turnController,
                  ConfigReader &configReader,
                  EconomyController &economyController);
    void run();
};

