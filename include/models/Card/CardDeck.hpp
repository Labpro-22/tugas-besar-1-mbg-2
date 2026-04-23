#pragma once
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
using namespace std;

template <class T>
class CardDeck
{
private:
    vector<T*> mainDeck;
    vector<T*> discardDeck;

    void shuffleMainDeck() {
        unsigned base = chrono::system_clock::now().time_since_epoch().count();
        default_random_engine engine(base);
        shuffle(mainDeck.begin(), mainDeck.end(). engine);
    }

public:
    ~CardDeck() {
        for (T* card : mainDeck) {
            delete card;
        }
        for (T* card : discardDeck) {
            delete card;
        }
    }

    void add(T* card) {
        mainDeck.push_back(card);
    }

    void initShuffle() {
        shuffleMainDeck();
    }

    T* draw() {
        if (mainDeck.empty()) {
            mainDeck = discardDeck;
            discardDeck.clear();
            shuffleMainDeck();
        }
        T* drawnCard = mainDeck.back();
        mainDeck.pop_back();
        return drawnCard;
    }

    void discard(T* card) {
        discardDeck.push_back(card);
    }

    void returnAndShuffle(T* card) {
        mainDeck.push_back(card);
        shuffleMainDeck();
    }
};