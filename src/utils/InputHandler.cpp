#include "InputHandler.hpp"
#include <algorithm>
#include <cctype>

const map<string, CommandType> InputHandler::commandMap = {
    {"LEMPAR_DADU", CommandType::LEMPAR_DADU},
    {"ATUR_DADU", CommandType::ATUR_DADU},
    {"CETAK_PAPAN", CommandType::CETAK_PAPAN},
    {"CETAK_AKTA", CommandType::CETAK_AKTA},
    {"CETAK_PROPERTI", CommandType::CETAK_PROPERTI},
    {"GADAI", CommandType::GADAI},
    {"TEBUS", CommandType::TEBUS},
    {"BANGUN", CommandType::BANGUN},
    {"GUNAKAN_KEMAMPUAN", CommandType::GUNAKAN_KEMAMPUAN},
    {"SIMPAN", CommandType::SIMPAN},
    {"MUAT", CommandType::MUAT},
    {"CETAK_LOG", CommandType::CETAK_LOG}
};

InputHandler::InputHandler(istream& input) : inputSource(input), lastStringInput(""), valInt1(0), valInt2(0) {}

CommandType InputHandler::getCommand() {
    getStringInput();
    string commands = lastStringInput;
    transform(commands.begin(), commands.end(), commands.begin(), [](unsigned char ch) {
        return static_cast<char>(toupper(ch));
    });

    auto i = commandMap.find(commands);
    if (i == commandMap.end()) {
        return CommandType::UNKNOWN_COMMAND;
    }
    return i->second;
}

void InputHandler::getIntInput() {
    inputSource >> valInt1;
    
    if (inputSource.fail()) {
        clearInputBuffer();
        valInt1 = -1; 
    }
}

void InputHandler::getStringInput() {
    inputSource >> lastStringInput;
}

void InputHandler::clearInputBuffer() {
    inputSource.clear();
    char c;
    while (inputSource.get(c) && c != '\n');
}

string InputHandler::getLastStringInput() const {
    return lastStringInput;
}

int InputHandler::getIntValue1() const {
    return valInt1;
}

int InputHandler::getIntValue2() const {
    return valInt2;
}