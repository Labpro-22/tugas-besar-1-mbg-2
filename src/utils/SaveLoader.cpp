#include "SaveLoader.hpp"

#include <fstream>
#include <iostream>

void SaveLoader::saveGame(string fileName, GameContext &gameContext) {
    ofstream out(fileName);
    if (!out.is_open()) {
        cout << "Gagal menyimpan game ke file: " << fileName << endl;
        return;
    }

    out << gameContext.getCurrentTurn() << " " << gameContext.getMaxTurns() << "\n";
    out << gameContext.getCurrentPlayerIndex() << "\n";

    vector<Player> players = gameContext.getPlayers();
    out << players.size() << "\n";
    for (Player &player : players) {
        string status = player.getStatus() == PlayerStatus::ACTIVE ? "ACTIVE" :
                        player.getStatus() == PlayerStatus::JAILED ? "JAILED" : "BANKRUPT";
            
        out << player.getName() << " " << player.getBalance() << " " << player.getPosition() << " "
            << status << " " << player.getJailTurns() << "\n";
        
        out << player.getSkillCardCount() << "\n";
        for (SkillCard* card : player.getSkillCard())
        {
            
        }
        
    }


    out.close();
    cout << "Game tersimpan ke " << fileName << endl;
}

void SaveLoader::loadGame(string fileName, GameContext &gameContext) {
    ifstream in(fileName);
    if (!in.is_open()) {
        cout << "Gagal memuat save dari file: " << fileName << endl;
        return;
    }

    int turn = 1;
    int currentIndex = 0;
    int playerCount = 0;

    in >> turn;
    in >> currentIndex;
    in >> playerCount;

    gameContext.setCurrentTurn(turn);
    gameContext.setCurrentPlayerIndex(currentIndex);

    vector<Player> players = gameContext.getPlayers();
    for (int i = 0; i < playerCount && i < players.size(); ++i) {
        string ignoredName;
        int balance = 0;
        int position = 0;
        int statusInt = 0;
        int jailTurns = 0;

        in >> ignoredName >> balance >> position >> statusInt >> jailTurns;

        players[i].setPosition(position);
        players[i].setStatus(static_cast<PlayerStatus>(statusInt));
        players[i].setJailTurns(jailTurns);

        int delta = balance - players[i].getBalance();
        players[i] += delta;
    }

    in.close();
    cout << "Game dimuat dari " << fileName << endl;
}

bool SaveLoader::checkSaveExists(string fileName) {
    ifstream in(fileName);
    return in.good();
}