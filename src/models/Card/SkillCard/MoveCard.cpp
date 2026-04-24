#include "MoveCard.hpp"
#include <random>

static mt19937& getRNG() {
    static random_device rd;
    static mt19937 gen(rd());
    return gen;
}

MoveCard::MoveCard() : SkillCard("Move", "Move forward random spaces.") {
    uniform_int_distribution<> dist(1, 12);
    this->steps = dist(getRNG());
    
    this->description = "Move forward " + to_string(this->steps) + " tile(s).";
}

MoveCard::MoveCard(int steps) : SkillCard ("Move", "Move forward random spaces."), steps(steps) {}

SkillCardType MoveCard::getSkillType() const { 
    return SkillCardType::MOVE; 
}

int MoveCard::getSteps() const { 
    return steps; 
}