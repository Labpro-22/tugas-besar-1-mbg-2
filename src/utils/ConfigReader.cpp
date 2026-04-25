#include "ConfigReader.hpp"
#include <fstream>
#include <sstream>
#include <map>

ConfigReader::ConfigReader(string filePath) : configFilePath{filePath}{}

void ConfigReader::loadAllConfigs(GameContext *gameContext, GameBoard &gameBoard){
    loadAksi("config/aksi.txt", gameBoard);
    loadProperty("config/property.txt", gameBoard);
    loadRailroad(gameContext, "config/railroad.txt");
    loadUtility(gameContext, "config/utility.txt");
    loadSpecial(gameContext, "config/special.txt");
    loadTax(gameContext, "config/tax.txt");
    loadMisc(gameContext, "config/misc.txt");
    auto props = gameBoard.getPropertyTile();
    
    props.erase(std::remove(props.begin(), props.end(), nullptr), props.end());

    std::sort(props.begin(), props.end(), [](PropertyTile* a, PropertyTile* b) {
        // Double check: Pastikan pointer valid
        if (a == nullptr || b == nullptr) return false; 
        return a->getIdx() < b->getIdx();
    });
}
void ConfigReader::loadAksi(string fileName, GameBoard &gameBoard) {
    ifstream file(fileName);
    if (!file.is_open()) return;
    string line;

    while (getline(file, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        int idx;
        string code, name, type, color;

        if (!(ss >> idx >> code >> name >> type >> color)) continue;

        if (type == "KARTU") {
            if (code == "KSP") {
                gameBoard.addTile(new CardTile(idx, code, name, CardType::CHANCE, color));
            } else if (code == "DNU") {
                gameBoard.addTile(new CardTile(idx, code, name, CardType::COMMUNITY_CHEST, color));
            }
        } 
        else if (type == "FESTIVAL") {
            gameBoard.addTile(new FestivalTile(idx, code, name, color));
        } 
        else if (type == "PAJAK") {
            bool isPPH = (code == "PPH");
            gameBoard.addTile(new TaxTile(idx, code, name, isPPH, color));
        } 
        else if (type == "SPESIAL") {
            if (code == "GO") {
                gameBoard.addTile(new StartTile(idx, code, name, color));
            } else if (code == "PEN") {
                gameBoard.addTile(new JailTile(idx, code, name, color));
            } else if (code == "BBP") {
                gameBoard.addTile(new FreeParkTile(idx, code, name, color));
            } else if (code == "PPJ") {
                gameBoard.addTile(new GoToJailTile(idx, code, name, color));
            }
        }
    }
}

void ConfigReader::loadProperty(string fileName, GameBoard &gameBoard){
    ifstream file(fileName);
    if (!file.is_open()) return;
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        int idx, price, morgageValue, houseCost, hotelCost;
        vector<int> rent(6);
        string code, name, type, color;

        if (!(ss >> idx >> code >> name >> type >> color >> price >> morgageValue)) {
            continue; 
        }
        
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

void ConfigReader::loadRailroad(GameContext *gameContext, string fileName){
    ifstream file(fileName);
    if (!file.is_open()) return;
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        int totalOwned, rent;
        if (!(ss >> totalOwned >> rent)) {
            continue;
        }
        gameContext->setRailroadRent(totalOwned, rent);
    }
};

void ConfigReader::loadUtility(GameContext *gameContext, string fileName){
    ifstream file(fileName);
    if (!file.is_open()) return;
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        int totalOwned, multiplier;
        if (!(ss >> totalOwned >> multiplier)) {
            continue;
        }
        gameContext->setUtilityMultiplier(totalOwned, multiplier);
    }
};

void ConfigReader::loadSpecial(GameContext *gameContext, string fileName){
    ifstream file(fileName);
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        int goSalary, jailFine;
        if(!(ss >> goSalary >> jailFine)){
            continue;
        }
        gameContext->setGoSalary(goSalary);
        gameContext->setJailFine(jailFine);
    }
};

void ConfigReader::loadTax(GameContext *gameContext, string fileName){
    ifstream file(fileName);
    if (!file.is_open()) return;
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        int pphFlat, pphPercentage, pbm;
        if (!(ss >> pphFlat >> pphPercentage >> pbm)) {
            continue;
        }
        gameContext->setPphFlat(pphFlat);
        gameContext->setPphPercentage(pphPercentage);
        gameContext->setPbm(pbm);
    }
};

void ConfigReader::loadMisc(GameContext* gameContext, string fileName){
    ifstream file(fileName);
    if (!file.is_open()) return;
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        int maxTurns, startingMoney; 
        if (!(ss >> maxTurns >> startingMoney)) {
            continue;
        }
        gameContext->setMaxTurns(maxTurns);
        gameContext->setStartingMoney(startingMoney);
    }
};
