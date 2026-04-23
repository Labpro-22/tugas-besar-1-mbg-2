#include "../include/core/GameEngine.hpp"

void GameEngine::initGame(GameBoard& gameBoard,
                          list<Player>& players,
                          CardDeck<Card>& specialCards,
                          GameContext& gameContext,
                          ConfigReader& configReader,
                          SaveLoader& saveLoader,
                          TurnController& turnController,
                          EconomyController& economyController)
{
    configReader.loadAllConfigs(gameBoard, economyController, turnController);
}

void GameEngine::run()
{
    GameBoard gameBoard;
    list<Player> players;
    CardDeck<Card> specialCards;
    GameContext gameContext(gameBoard,players,specialCards);
    ConfigReader configReader;
    SaveLoader saveLoader;
    TurnController turnController;
    EconomyController economyController;

    initGame(gameBoard,players,
            specialCards,gameContext,
            configReader,saveLoader,
            turnController,
            economyController);
    // TODO: game loop
}


