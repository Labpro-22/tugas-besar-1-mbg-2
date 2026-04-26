#pragma once
#include <vector>
#include <algorithm>
#include <set>
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
        shuffle(mainDeck.begin(), mainDeck.end(), engine);
    }

public:
    ~CardDeck() = default;

    void add(T* card) {
        mainDeck.push_back(card);
    }

    void initShuffle() {
        shuffleMainDeck();
    }

    T* draw() {
        if (mainDeck.empty()) {
            if (discardDeck.empty()) {
                return nullptr; 
            }
            mainDeck = std::move(discardDeck);
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
    
    const vector<T*>& getMainDeck() const {
        return mainDeck;
    }

    const vector<T*>& getDiscardDeck() const {
        return discardDeck;
    }
};