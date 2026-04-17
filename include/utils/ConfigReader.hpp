#pragma once
#include <string>
using namespace std;

class ConfigReader
{
private:
    string configFilePath;
public:
    void loadAllConfigs();
    void loadProperty(string fileName);
    void loadRailroad(string fileName);
    void loadUtility(string fileName);
    void loadSpecial(string fileName);
    void loadTax(string fileName);
    void loadMisc(string fileName);
};

