#include "MoveCard.hpp"
#include <random>

static mt19937& getRNG() {
    static random_device rd;
    static mt19937 gen(rd());
    return gen;
}

MoveCard::MoveCard() : SkillCard("Kartu Maju", "Bergerak maju sejumlah petak secara acak.") {
    uniform_int_distribution<> dist(1, 10);
    this->steps = dist(getRNG());
    
    this->description = "Bergerak maju " + to_string(this->steps) + " petak.";
}

SkillCardType MoveCard::getSkillType() const { 
    return SkillCardType::MOVE; 
}

int MoveCard::getSteps() const { 
    return steps; 
}