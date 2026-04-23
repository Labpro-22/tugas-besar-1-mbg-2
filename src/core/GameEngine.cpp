#include "include/core/GameEngine.hpp"

void GameEngine:: initGame(GameContext& gameContext,
                          TurnController& turnController,
                          ConfigReader& configReader,
                          EconomyController& economyController)
{
    configReader.loadAllConfigs(gameContext.getBoard(), economyController, turnController);
}

void GameEngine::run()
{
    string filePath = "Ayam";
    GameContext gameContext;
    ConfigReader configReader(filePath);
    SaveLoader saveLoader;
    TurnController turnController;
    EconomyController economyController;

    initGame(gameContext,
             turnController,
             configReader,
             economyController);
    // TODO: game loop
}


