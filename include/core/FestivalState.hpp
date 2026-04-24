#pragma once

using namespace std;

class FestivalState
{
private:
    int stacks = 0;
    int turnsLeft = 0;

public:
    FestivalState();
    void activateFestival();
    void increaseStack();
    void resetDuration();
    void decrementTurn();

    bool isExpired() const;
    int getStacks() const;
    int getTurnsLeft() const;
    void setStacks(int newStacks);
    void setTurnsLeft(int newTurnsLeft);
};
