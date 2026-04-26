#pragma once
#include "GameBoard.hpp"
#include "../utils/ConfigReader.hpp"
#include "GameContext.hpp"
#include "SaveLoader.hpp"
#include "TurnController.hpp"
#include <string>
#include "AllCards.hpp"
#include "DisplayView.hpp" 
#include "GameView.hpp"

using namespace std;
class GameEngine
{
private: 
    DisplayView cliView;    // Tampilan Terminal 
    GameView* guiView;      // Tampilan Window SFML 
    bool isGUIMode;         // Penanda apakah GUI diaktifkan

public:
    GameEngine() : guiView(nullptr), isGUIMode(false) {}

    GameEngine(GameView* gui) : guiView(gui), isGUIMode(true) {}
    
    void initGame(GameContext &gameContext,
                  TurnController &turnController,
                  ConfigReader &configReader,
                  EconomyController &economyController);
    void run();
};

