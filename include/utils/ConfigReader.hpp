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
    void loadAllConfigs(GameBoard &gameBoard, EconomyController &economyController, TurnController &turnController);
    void loadProperty(string fileName, GameBoard &gameBoard);
    void loadRailroad(string fileName, EconomyController &economyController);
    void loadUtility(string fileName, EconomyController &economyController);
    void loadSpecial(string fileName, EconomyController &economyController);
    void loadTax(string fileName, EconomyController &economyController);
    void loadMisc(string fileName, TurnController &turnController);
};
    
