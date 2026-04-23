#include "SaveLoader.hpp"
#include "Card.hpp"
#include "DiscountCard.hpp"
#include "MoveCard.hpp"
#include "ShieldCard.hpp"
#include "GameContext.hpp"
#include <fstream>
#include <iostream>

void SaveLoader::saveGame(string fileName, GameContext &gameContext) {
    ofstream out(fileName);
    if (!out.is_open()) {
        cout << "Gagal menyimpan game ke file: " << fileName << endl;
        return;
    }

    // Turn 
    out << gameContext.getCurrentTurn() << " " << gameContext.getMaxTurns() << "\n";

    // Total Player
    vector<Player> players = gameContext.getPlayers();
    out << players.size() << "\n";

    // State Pemain
    for (Player &player : players) {
        string status = player.getStatus() == PlayerStatus::ACTIVE ? "ACTIVE" :
                        player.getStatus() == PlayerStatus::JAILED ? "JAILED" : "BANKRUPT";
            
        out << player.getName() << " " << player.getBalance() << " " << player.getPosition() << " "
            << status << " " << player.getJailTurns() << "\n";
        
        out << player.getSkillCardCount() << "\n";
        for (SkillCard* card : player.getSkillCard())
        {
            out << card->getName();
            if (card->getSkillType() == SkillCardType::DISCOUNT) {
                out << " " << static_cast<DiscountCard*>(card)->getDiscountPercentage();
            }else if(card->getSkillType() == SkillCardType::MOVE) {
                out << " " << static_cast<MoveCard*>(card)->getSteps();
            }
            out << "\n";
        }
    }


    // Player Turn Order
    for (Player &player : players)
    {
        out << player.getName();
    }

    out << "\n";

    // Current player in this turn
    out << gameContext.getCurrentPlayer().getName() << "\n";

    // Properti State
    out << gameContext.getBoard().getPropertyTile().size() << "\n";
    for (PropertyTile* prop : gameContext.getBoard().getPropertyTile())
    {
        string typeLabel = prop->getPropertyType() == PropertyType::STREET ? "STREET" :
                           prop->getPropertyType() == PropertyType::RAILROAD ? "RAILROAD" : "UTILITY";
        string ownerName = prop->isOwned() ? prop->getOwner()->getName() : "BANK";
        string status = prop->getStatus() == OWNED ? "OWNED" : 
                        prop->getStatus() == MORTGAGED ? "MORTGAGED" : "BANK";
        
        string fmult = "1";
        string fdur = "0";
        string nBuilding = "0";

        if (prop->getPropertyType() == STREET) {
            StreetTile* street = dynamic_cast<StreetTile*>(prop);
            fmult = to_string(street->getFestivalStack());
            fdur = to_string(street->getFestivalTurn());
            if (street->getHasHotel()) {
                nBuilding = "H"; // 5 untuk hotel
            } else {
                nBuilding = to_string(street->getHouseCount());
            }
        }
        
        out << prop->getCode() << " " << typeLabel << " " << ownerName << " " << status << " " << fmult << " " << fdur << " " << nBuilding << "\n";
    }


    for (const auto& card : gameContext.getSkillCards())
    {
        
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