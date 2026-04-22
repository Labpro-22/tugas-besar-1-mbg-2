#pragma once

class Dice
{
private:
    int dice1 = 1;
    int dice2 = 1;
public:
    void roll();
    void setRoll(int x, int y);
    bool isDouble() const;
    int getTotal() const;
    int getDice1() const;
    int getDice2() const;
};

