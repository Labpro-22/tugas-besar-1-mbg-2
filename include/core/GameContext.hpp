#pragma once
#include <vector>
#include <map>
#include <iostream>

#include "Player.hpp"
#include "CardDeck.hpp"
#include "ActionCard.hpp"
#include "SkillCard.hpp"
#include "PropertyTile.hpp"
#include "FestivalState.hpp"
#include "Dice.hpp"
#include "GameBoard.hpp"
using namespace std;

class GameContext
{
private:
    GameBoard board;
    vector<Player> players;
    CardDeck<ActionCard> chanceDeck;
    CardDeck<ActionCard> communityChestDeck;
    CardDeck<SkillCard> skillDeck;
    Dice dice;

    // TURN
    int currentPlayerIndex = 0;
    int currentTurn = 1;
    int maxTurns = 0;
    int startingMoney = 0;

    // ECONOMY
    int goSalary = 0;
    int pphFlat = 0;
    int pphPercentage = 0;
    int pbm = 0;
    int jailFine = 0;
    map<int, int> railroadRent; // key: jumlah railroad yang dimiliki, value: rent
    map<int, int> UtilityMultiplier; // key: jumlah utility yang dimiliki, value: multiplier

    bool gameOver = false;
    int winnerIndex = -1;


public:
    GameContext() = default;

    GameBoard& getBoard() ;
    vector<Player>& getPlayers();
    CardDeck<ActionCard>& getChanceDeck();
    CardDeck<ActionCard>& getCommunityChestDeck();
    CardDeck<SkillCard>& getSkillDeck();
    Dice& getDice();

    int getCurrentPlayerIndex() const;
    int getCurrentTurn() const;
    int getMaxTurns() const;
    int getStartingMoney() const;
    int getGoSalary() const;
    int getPphFlat() const;
    int getPphPercentage() const;
    int getPbm() const;
    int getJailFine() const;
    bool isGameOver() const;
    int getWinnerIndex() const; 
    
    const map<int, int> &getRailroadRentTable() const;
    const map<int, int> &getUtilityMultiplierTable() const;
    void setCurrentPlayerIndex(int index);
    void setCurrentTurn(int turn);
    void setMaxTurns(int turns);
    void setStartingMoney(int money);
    void setGoSalary(int amount);
    void setPphFlat(int amount);
    void setPphPercentage(int amount);
    void setPbm(int amount);
    void setJailFine(int amount);
    void setRailroadRent(int totalOwned, int rent);
    void setUtilityMultiplier(int totalOwned, int multiplier);
    void setGameOver(bool value);
    void setWinnerIndex(int index);
    void clearAllFestivalStates();
    Player& getCurrentPlayer();
    void nextPlayer();
};
