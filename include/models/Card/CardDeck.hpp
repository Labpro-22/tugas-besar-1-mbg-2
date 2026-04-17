#pragma once
#include <deque>

using namespace std;
template <class T>
class CardDeck
{
private:
    deque<T> cards;
public:
    void addCard(T card);
    void shuffle();
    
    T* drawCard();

    void returnCard(T* card);
    bool isEmpty();

    virtual ~CardDeck();
};