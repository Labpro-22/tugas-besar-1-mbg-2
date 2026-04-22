#pragma once
#include <iostream>
#include <string>
#include <map>
using namespace std;

enum class CommandType {
    LEMPAR_DADU,
    ATUR_DADU,
    CETAK_PAPAN,
    CETAK_AKTA,
    CETAK_PROPERTI,
    GADAI,
    TEBUS,
    BANGUN,
    GUNAKAN_KEMAMPUAN,
    SIMPAN,
    MUAT,
    CETAK_LOG,
    UNKNOWN_COMMAND
};

class InputHandler
{
private:
    istream &inputSource;
    string lastStringInput;
    int valInt1;
    int valInt2;
    static const map<string, CommandType> commandMap;
public:
    InputHandler(istream& input = cin);
    CommandType getCommand();
    void getIntInput();
    void getStringInput();
    void clearInputBuffer();

    string getLastStringInput() const;
    int getIntValue1() const;
    int getIntValue2() const;
};

