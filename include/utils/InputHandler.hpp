#pragma once
#include <iostream>
#include <string>

using namespace std;

class InputHandler
{
private:
    istream* inputSource;
    string lastStringInput;
    int lastIntInput;

    InputHandler();

public:
    static InputHandler& getInstance();

    InputHandler(const InputHandler&) = delete;
    InputHandler& operator=(const InputHandler&) = delete;

    void setInputSource(istream& source);

    int getIntInput(const string& prompt = "");
    string getStringInput(const string& prompt = "");
    bool getYesNoInput(const string& prompt = "");

    int getLastIntInput() const;
    string getLastStringInput() const;

    void clearInputBuffer();
};

