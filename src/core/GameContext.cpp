#include "GameContext.hpp"

#include <iterator>

GameContext::GameContext(GameBoard &board, list<Player> &players, CardDeck<Card> &specialCards)
    : board(board),
      players(players),
      specialCards(specialCards),
      currentPlayerIndex(0),
      currentTurn(1),
      maxTurns(0),
      startingMoney(0),
      goSalary(0),
      pph(0),
      pbm(0),
      jailFine(0),
      gameOver(false),
      winnerIndex(-1) {}

GameBoard& GameContext::getBoard() const { return this->board; }
list<Player>& GameContext::getPlayers() const { return this->players; }
CardDeck<Card>& GameContext::getSpecialCards() const { return this->specialCards; }

int GameContext::getCurrentPlayerIndex() const { return currentPlayerIndex; }
int GameContext::getCurrentTurn() const { return currentTurn; }
int GameContext::getMaxTurns() const { return maxTurns; }
int GameContext::getStartingMoney() const { return startingMoney; }
int GameContext::getGoSalary() const { return goSalary; }
int GameContext::getPph() const { return pph; }
int GameContext::getPbm() const { return pbm; }
int GameContext::getJailFine() const { return jailFine; }
bool GameContext::isGameOver() const { return gameOver; }
int GameContext::getWinnerIndex() const { return winnerIndex; }
const map<int, int> &GameContext::getRailroadRentTable() const { return this->railroadRent; }
const map<int, int> &GameContext::getUtilityMultiplierTable() const { return this->UtilityMultiplier; }
const map<PropertyTile*, FestivalState> &GameContext::getFestivalStates() const { return this->festivalStates; }

bool GameContext::hasFestivalState(PropertyTile* tile) const{
    return festivalStates.find(tile) != festivalStates.end();
}

FestivalState GameContext::getFestivalState(PropertyTile* tile) const{
    auto it = festivalStates.find(tile);
    if (it != festivalStates.end())
        return it->second;
    return FestivalState();
}

void GameContext::setCurrentPlayerIndex(int index) { currentPlayerIndex = index; }
void GameContext::setCurrentTurn(int turn) { currentTurn = turn; }
void GameContext::setMaxTurns(int turns) { maxTurns = turns; }
void GameContext::setStartingMoney(int money) { startingMoney = money; }
void GameContext::setGoSalary(int amount) { goSalary = amount; }
void GameContext::setPph(int amount) { pph = amount; }
void GameContext::setPbm(int amount) { pbm = amount; }
void GameContext::setJailFine(int amount) { jailFine = amount; }
void GameContext::setRailroadRent(int totalOwned, int rent) { railroadRent[totalOwned] = rent; }
void GameContext::setUtilityMultiplier(int totalOwned, int multiplier) { UtilityMultiplier[totalOwned] = multiplier; }
void GameContext::setGameOver(bool value) { gameOver = value; }
void GameContext::setWinnerIndex(int index) { winnerIndex = index; }

void GameContext::setFestivalState(PropertyTile* tile, const FestivalState &state){
    festivalStates[tile] = state;
}
void GameContext::clearFestivalState(PropertyTile* tile){
    festivalStates.erase(tile);
}
void GameContext::clearAllFestivalStates(){
    festivalStates.clear();
}
Player& GameContext::getCurrentPlayer() const{
    auto it = players.begin();
    advance(it, currentPlayerIndex);
    return *it;
}
void GameContext::nextPlayer(){
    if (players.empty()) return;
    currentPlayerIndex = (currentPlayerIndex + 1) % int(players.size());
}