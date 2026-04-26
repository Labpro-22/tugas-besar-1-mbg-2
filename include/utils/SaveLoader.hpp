#pragma once
#include <string>
#include "GameContext.hpp"
#include "GameLogger.hpp"
using namespace std;

class SaveLoader
{
private:
    string saveFolderPath;
    string currentSaveName;
public:
    void saveGame(string fileName, GameContext &gameContext, GameLogger& logger);
    void loadGame(string fileName, GameContext &gameContext, GameLogger& logger);
    bool checkSaveExists(string fileName);
};
