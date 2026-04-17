#pragma once
#include <string>
using namespace std;

enum CardType
{
    ACTION,
    SKILL
};

class Card
{
protected:
    string cardName;
    string description;
    CardType type; 
public:
    Card(string name, string desc);
    string getName() const;
    string getDescription() const;
    virtual ~Card() = default; // virtual destructor
};
