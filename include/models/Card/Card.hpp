#pragma once
#include <string>
using namespace std;

enum CardCategory
{
    ACTION,
    SKILL,
};

class Card
{
protected:
    string name;
    string description;
public:
    Card(string name, string desc);
    virtual ~Card() = default;

    string getName() const;
    string getDescription() const;

    virtual CardCategory getCategory() const = 0;
};
