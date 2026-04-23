#include "Dice.hpp"

#include <algorithm>
#include <random>

static int setDice(int value) {
	return std::clamp(value, 1, 6);
}

void Dice::roll() {
	static std::mt19937 rng(std::random_device{}());
	static std::uniform_int_distribution<int> dist(1, 6);

	dice1 = dist(rng);
	dice2 = dist(rng);
}

void Dice::setRoll(int x, int y) {
	dice1 = setDice(x);
	dice2 = setDice(y);
}

bool Dice::isDouble() const {
	return dice1 == dice2;
}

int Dice::getTotal() const {
	return dice1 + dice2;
}

int Dice::getDice1() const {
	return dice1;
}

int Dice::getDice2() const {
	return dice2;
}

