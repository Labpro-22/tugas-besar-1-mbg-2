#include "FestivalState.hpp"

FestivalState::FestivalState(){}

void FestivalState::activateFestival(){
    stacks = 1;
    turnsLeft = 3;
}

void FestivalState::increaseStack(){
    if (stacks != 8){
        stacks *= 2;
    }
}
void FestivalState::resetDuration(){
    turnsLeft = 3;
}
void FestivalState::decrementTurn(){
    if (turnsLeft > 0 ){
        turnsLeft--;
    }
}
bool FestivalState::isExpired() const{
    return turnsLeft == 0;
}
int FestivalState::getStacks() const{
    return stacks;
}
int FestivalState::getTurnsLeft() const{
    return turnsLeft;
}

void FestivalState::setStacks(int newStacks){
    stacks = newStacks;
}

void FestivalState::setTurnsLeft(int newTurnsLeft){
    turnsLeft = newTurnsLeft;
}