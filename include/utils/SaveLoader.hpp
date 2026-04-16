#pragma once
#include <string>
using namespace std;

class SaveLoader
{
private:
    string saveFolderPath;
    string currentSaveName;
public:
    void saveGame(string fileName);
    void loadGame(string fileName);
    bool checkSaveExists(string fileName);
};
