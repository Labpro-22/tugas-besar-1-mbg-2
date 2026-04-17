#pragma once
#include <iostream>
#include <string>
using namespace std;

class InputHandler
{
private:
    istream &inputSource;
    string lastStringInput;
    int valInt1;
    int valInt2;
public:
    void getIntInput();
    void getStringInput();
    void clearInputBuffer();
};

