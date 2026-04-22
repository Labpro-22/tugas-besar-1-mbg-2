#pragma once
#include "GameBoard.hpp"
#include "include/utils/ConfigReader.hpp"
#include "GameContext.hpp"
#include "SaveLoader.hpp"
#include "TurnController.hpp"
#include <string>

using namespace std;
class GameEngine
{
public:
    void initGame(GameBoard &gameBoard,
                  list<Player> &players,
                  CardDeck<Card> &specialCards,
                  GameContext &gameContext,
                  ConfigReader &configReader,
                  SaveLoader &saveLoader,
                  TurnController &turnController,
                  EconomyController &economyController);
    void run();
};

