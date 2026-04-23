#include "GameContext.hpp"
#include <iterator>


GameBoard& GameContext::getBoard()  { return board; }
vector<Player>& GameContext::getPlayers()  { return players; }
CardDeck<Card>& GameContext::getSpecialCards()  { return specialCards; }

int GameContext::getCurrentPlayerIndex() const { return currentPlayerIndex; }
int GameContext::getCurrentTurn() const { return currentTurn; }
int GameContext::getMaxTurns() const { return maxTurns; }
int GameContext::getStartingMoney() const { return startingMoney; }
int GameContext::getGoSalary() const { return goSalary; }
int GameContext::getPphFlat() const {return pphFlat;}
int GameContext::getPphPercentage() const {return pphPercentage;}
int GameContext::getPbm() const { return pbm; }
int GameContext::getJailFine() const { return jailFine; }
bool GameContext::isGameOver() const { return gameOver; }
int GameContext::getWinnerIndex() const { return winnerIndex; }
const map<int, int> &GameContext::getRailroadRentTable() const { return this->railroadRent; }
const map<int, int> &GameContext::getUtilityMultiplierTable() const { return this->UtilityMultiplier; }

void GameContext::setCurrentPlayerIndex(int index) { currentPlayerIndex = index; }
void GameContext::setCurrentTurn(int turn) { currentTurn = turn; }
void GameContext::setMaxTurns(int turns) { maxTurns = turns; }
void GameContext::setStartingMoney(int money) { startingMoney = money; }
void GameContext::setGoSalary(int amount) { goSalary = amount; }
void GameContext::setPphFlat(int amount) { pphFlat = amount; }
void GameContext::setPphPercentage(int amount) { pphPercentage = amount; }
void GameContext::getPphFlat(int amount)  { setPphFlat(amount); }
void GameContext::getPphPercentage(int amount)  { setPphPercentage(amount); }
void GameContext::setJailFine(int amount) { jailFine = amount; }
void GameContext::setRailroadRent(int totalOwned, int rent) { railroadRent[totalOwned] = rent; }
void GameContext::setUtilityMultiplier(int totalOwned, int multiplier) { UtilityMultiplier[totalOwned] = multiplier; }
void GameContext::setGameOver(bool value) { gameOver = value; }
void GameContext::setWinnerIndex(int index) { winnerIndex = index; }

Player& GameContext::getCurrentPlayer() {
    auto it = players.begin();
    advance(it, currentPlayerIndex);
    return *it;
}
void GameContext::nextPlayer(){
    if (players.empty()) return;
    currentPlayerIndex = (currentPlayerIndex + 1) % int(players.size());
}