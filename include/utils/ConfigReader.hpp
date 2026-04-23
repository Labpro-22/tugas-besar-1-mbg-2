#pragma once
#include <string>
#include "GameBoard.hpp"
#include "EconomyController.hpp"
#include "TurnController.hpp"

// Action Tiles
#include "CardTile.hpp"
#include "FestivalTile.hpp"
#include "FreeParkTile.hpp"
#include "GoToJailTile.hpp"
#include "JailTile.hpp"
#include "StartTile.hpp"
#include "TaxTile.hpp"

// Property Tiles
#include "StreetTile.hpp"
#include "RailroadTile.hpp"
#include "UtilityTile.hpp"

using namespace std;

class ConfigReader
{
private:
    string configFilePath;
public:
    ConfigReader(string filePath);
    void loadAllConfigs(GameContext *gameContext, GameBoard &gameBoard, EconomyController &economyController, TurnController &turnController);
    void loadProperty(string fileName, GameBoard &gameBoard);
    void loadRailroad(GameContext *gameContext, string fileName, EconomyController &economyController);
    void loadUtility(GameContext *gameContext, string fileName, EconomyController &economyController);
    void loadSpecial(GameContext *gameContext, string fileName, EconomyController &economyController);
    void loadTax(GameContext *gameContext, string fileName, EconomyController &economyController);
    void loadMisc(string fileName, TurnController &turnController);
};
    
