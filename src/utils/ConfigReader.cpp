#include "ConfigReader.hpp"
#include <fstream>
#include <sstream>
#include <map>

ConfigReader::ConfigReader(string filePath) : configFilePath{filePath}{}

void ConfigReader::loadAllConfigs(GameContext *gameContext, GameBoard &gameBoard, EconomyController &economyController, TurnController &turnController){
    loadProperty("config/property.txt", gameBoard);
    loadRailroad(gameContext, "config/railroad.txt", economyController);
    loadUtility(gameContext, "config/utility.txt", economyController);
    loadSpecial(gameContext, "config/special.txt", economyController);
    loadTax(gameContext, "config/tax.txt", economyController);
    loadMisc("config/misc.txt", turnController);
}

void ConfigReader::loadProperty(string fileName, GameBoard &gameBoard){
    // masih hardcode untuk tile aksi dan special, karena tidak ada txt
    gameBoard.addTile(new StartTile(0, "GO", "Petak Mulai", "DEFAULT"));
    gameBoard.addTile(new CardTile(2, "DNU", "Dana Umum", CardType::DANA_UMUM, "DEFAULT"));
    gameBoard.addTile(new TaxTile(4, "PPH", "Pajak Penghasilan", true, "DEFAULT"));
    gameBoard.addTile(new FestivalTile(7, "FES", "Festival", "DEFAULT"));
    gameBoard.addTile(new JailTile(10, "PEN", "Penjara", "DEFAULT"));
    gameBoard.addTile(new CardTile(17, "DNU", "Dana Umum", CardType::DANA_UMUM, "DEFAULT"));
    gameBoard.addTile(new FreeParkTile(20, "BBP", "Pajak Bumi dan Bangunan", "DEFAULT"));
    gameBoard.addTile(new CardTile(22, "KSP", "Kesempatan", CardType::KESEMPATAN, "DEFAULT"));
    gameBoard.addTile(new GoToJailTile(30, "PPJ", "Petak Pergi ke Penjara", "DEFAULT"));
    gameBoard.addTile(new FestivalTile(33, "FES", "Festival", "DEFAULT"));
    gameBoard.addTile(new CardTile(36, "KSP", "Kesempatan", CardType::KESEMPATAN, "DEFAULT"));
    gameBoard.addTile(new TaxTile(38, "PBM", "Pajak Barang Mewah", false, "DEFAULT"));

    ifstream file(fileName);
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        int idx, price, morgageValue, houseCost, hotelCost;
        vector<int> rent(6);
        string code, name, type, color;
        ss >> idx >> code >> name >> type >> color >> price >> morgageValue;
        
        if(type == "STREET"){
            ss >> houseCost >> hotelCost;
            for(int i = 0; i < 6; i++) ss >> rent[i];

            gameBoard.addTile(new StreetTile(idx, code, name, price, morgageValue, rent,houseCost, hotelCost,  color));
        }else if(type == "RAILROAD"){
            gameBoard.addTile(new RailroadTile(idx, code, name, price, morgageValue, color));
        }else if(type == "UTILITY"){
            gameBoard.addTile(new UtilityTile(idx, code, name, price, morgageValue, color));
        }
    }

}

void ConfigReader::loadRailroad(GameContext *gameContext, string fileName, EconomyController &economyController){
    ifstream file(fileName);
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        int totalOwned, rent;
        ss >> totalOwned >> rent;
        economyController.addRailroadRent(gameContext, totalOwned, rent);
    }
};

void ConfigReader::loadUtility(GameContext *gameContext, string fileName, EconomyController &economyController){
    ifstream file(fileName);
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        int totalOwned, multiplier;
        ss >> totalOwned >> multiplier;
        economyController.addUtilityMultiplier(gameContext, totalOwned, multiplier);
    }
};

void ConfigReader::loadSpecial(GameContext *gameContext, string fileName, EconomyController &economyController){
    ifstream file(fileName);
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        int goSalary, jailFine;
        ss >> goSalary >> jailFine;
        economyController.setGoSalary(gameContext, goSalary);
        economyController.setJailFine(gameContext, jailFine);
    }
};

void ConfigReader::loadTax(GameContext *gameContext, string fileName, EconomyController &economyController){
    ifstream file(fileName);
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        int pphFlat, pphPercentage, pbm;
        ss >> pphFlat >> pphPercentage >> pbm;
        economyController.setPphFlat(gameContext, pphFlat);
        economyController.setPphPercentage(gameContext, pphPercentage);
        economyController.setPbm(gameContext, pbm);
    }
};

void ConfigReader::loadMisc(string fileName, TurnController &turnController){
    ifstream file(fileName);
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        int maxTurns, startingMoney; 
        ss >> maxTurns >> startingMoney;
        turnController.setMaxTurns(maxTurns);
        turnController.setStartingMoney(startingMoney);
    }
};
