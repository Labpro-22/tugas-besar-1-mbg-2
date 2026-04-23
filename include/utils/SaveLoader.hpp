#pragma once
#include <string>
#include "GameContext.hpp"
using namespace std;

class SaveLoader
{
private:
    string saveFolderPath;
    string currentSaveName;
public:
    void saveGame(string fileName, GameContext &gameContext);
    void loadGame(string fileName, GameContext &gameContext);
    bool checkSaveExists(string fileName);
};
