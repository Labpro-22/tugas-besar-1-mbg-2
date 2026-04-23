#pragma once
#include <string>
using namespace std;

class Card
{
protected:
    string name;
    string description;
    
    Card(string name, string desc);
public:
    virtual ~Card() = default;

    string getName() const;
    string getDescription() const;
};
