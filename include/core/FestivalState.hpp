#pragma once

using namespace std;

class FestivalState
{
private:
    int stacks;
    int turnsLeft;

public:
    FestivalState(int stacks = 1, int turnsLeft = 3);

    void increaseStack();
    void resetDuration();
    void decrementTurn();

    bool isExpired() const;
    int getStacks() const;
    int getTurnsLeft() const;
};
