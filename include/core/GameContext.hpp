#pragma once

#include <list>
#include <map>
#include <iostream>

#include "GameBoard.hpp"
#include "Player.hpp"
#include "CardDeck.hpp"
#include "Card.hpp"
#include "PropertyTile.hpp"
#include "FestivalState.hpp"


class GameContext
{
private:
    GameBoard* board;
    std::list<Player>* players;
    CardDeck<Card>* specialCards;

    
    // TURN
    int currentPlayerIndex;
    int currentTurn;
    int maxTurns;
    int startingMoney;


    // ECONOMY
    int goSalary;
    int pph;
    int pbm;
    int jailFine;
    map<int, int> railroadRent; // key: jumlah railroad yang dimiliki, value: rent
    map<int, int> UtilityMultiplier; // key: jumlah utility yang dimiliki, value: multiplier

    bool gameOver;
    int winnerIndex;

    // Effect
    std::map<PropertyTile*, FestivalState> festivalStates;

public:
    GameContext();

    void setBoard(GameBoard* board);
    void setPlayers(std::list<Player>* players);
    void setSpecialCards(CardDeck<Card>* specialCards);

    GameBoard* getBoard() const;
    std::list<Player>* getPlayers() const;
    CardDeck<Card>* getSpecialCards() const;

    int getCurrentPlayerIndex() const;
    int getCurrentTurn() const;
    int getMaxTurns() const;
    int getStartingMoney() const;
    int getJailFine() const;

    bool isGameOver() const;
    int getWinnerIndex() const;

    const std::map<PropertyTile*, FestivalState>& getFestivalStates() const;
    bool hasFestivalState(PropertyTile* tile) const;
    FestivalState getFestivalState(PropertyTile* tile) const;

    void setCurrentPlayerIndex(int index);
    void setCurrentTurn(int turn);
    void setMaxTurns(int turns);
    void setStartingMoney(int money);
    void setGameOver(bool value);
    void setWinnerIndex(int index);

    void setFestivalState(PropertyTile* tile, const FestivalState& state);
    void clearFestivalState(PropertyTile* tile);
    void clearAllFestivalStates();

    Player* getCurrentPlayer() const;
    void nextPlayer();
};
