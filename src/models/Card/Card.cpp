#include "Card.hpp"

Card::Card(string name, string desc) : name(name), description(desc) {}

string Card::getName() const {
    return name;
}

string Card::getDescription() const {
    return description;
}