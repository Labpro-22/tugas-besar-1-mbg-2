#pragma once

class Dice
{
private:
    int dice1;
    int dice2;
public:
    void roll();
    void setRoll(int x, int y);
    bool isDouble();
    int getTotal();
    int getDice1();
    int getDice2();
};

