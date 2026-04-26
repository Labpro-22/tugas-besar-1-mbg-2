#include "SaveLoader.hpp"
#include "Card.hpp"
#include "DiscountCard.hpp"
#include "MoveCard.hpp"
#include "ShieldCard.hpp"
#include "MoveCard.hpp"
#include "DiscountCard.hpp"
#include "TeleportCard.hpp"
#include "LassoCard.hpp"
#include "DemolitionCard.hpp"
#include "GameContext.hpp"
#include <fstream>
#include <iostream>

void SaveLoader::saveGame(string fileName, GameContext &gameContext, GameLogger& logger) {
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
        out << player.getName() << " ";
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

    // Deck State 
    CardDeck<SkillCard>& skillDeck = gameContext.getSkillDeck();
    out << skillDeck.getMainDeck().size() << "\n";
    for (const auto& card : skillDeck.getMainDeck())
    {   
        out << card->getName() << "\n";
    }

    out << skillDeck.getDiscardDeck().size() << "\n";
    for (const auto& card : skillDeck.getDiscardDeck())
    {
        out << card->getName() << "\n";
    }

    logger.saveToStream(out);

    out.close();
    cout << "Game tersimpan ke " << fileName << endl;
}

void SaveLoader::loadGame(string fileName, GameContext &gameContext, GameLogger& logger) {
    ifstream in(fileName);
    if (!in.is_open()) {
        cout << "Gagal memuat save dari file: " << fileName << endl;
        return;
    }

    int turn = 1;
    int maxTurns = 0;
    int currentIndex = 0;
    int playerCount = 0;

    in >> turn >> maxTurns;
    in >> playerCount;

    gameContext.setCurrentTurn(turn);
    gameContext.setMaxTurns(maxTurns);

    vector<Player>& players = gameContext.getPlayers();
    for (int i = 0; i < playerCount && i < players.size(); ++i) {
        string username;
        int balance = 0;
        int position = 0;
        int statusInt = 0;
        int jailTurns = 0;

        in >> username >> balance >> position >> statusInt >> jailTurns;

        players[i].setName(username);
        players[i].setBalance(balance);
        players[i].setPosition(position);
        players[i].setStatus(static_cast<PlayerStatus>(statusInt));
        players[i].setJailTurns(jailTurns);

        int skillCardCount = 0;
        in >> skillCardCount;

        for (int j = 0; j < skillCardCount; ++j) {
            string cardName;
            in >> cardName;

            SkillCard* card = nullptr;
            if (cardName == "DiscountCard") {
                int discountPercentage;
                in >> discountPercentage;
                card = new DiscountCard(discountPercentage);
            } else if (cardName == "MoveCard") {
                int steps;
                in >> steps;
                card = new MoveCard(steps);
            } else if (cardName == "ShieldCard") {
                card = new ShieldCard();
            } else if (cardName == "TeleportCard") {
                card = new TeleportCard();
            } else if (cardName == "LassoCard") {
                card = new LassoCard();
            } else if (cardName == "DemolitionCard") {
                card = new DemolitionCard();
            }

            if (card != nullptr) {
                players[i].addCardToHand(card);
            }
        }
    }

    for (int i = 0; i < playerCount; ++i) {
        string turnName;
        in >> turnName; 
    }

    string activePlayerName;
    in >> activePlayerName;

    int activeIndex = 0;
    for (int i = 0; i < players.size(); ++i) {
        if (players[i].getName() == activePlayerName) {
            activeIndex = i;
            break;
        }
    }
    
    gameContext.setCurrentPlayerIndex(activeIndex);

    int totalProperties;
    in >> totalProperties;
    for (int i = 0; i < totalProperties; ++i) {
        string code, typeLabel, ownerName, status;
        string festivalMult, festivalDur, nBuilding;

        in >> code >> typeLabel >> ownerName >> status >> festivalMult >> festivalDur >> nBuilding;

        PropertyTile* prop = gameContext.getBoard().getPropertyTileByCode(code);
        if (prop != nullptr) {
            if (ownerName != "BANK") {
                for (Player& player : players) {
                    if (player.getName() == ownerName) {
                        prop->setOwner(&player);
                        break;
                    }
                }
            }

            if (status == "MORTGAGED") {
                prop->setStatus(MORTGAGED);
            } else if (status == "OWNED") {
                prop->setStatus(OWNED);
            } else {
                prop->setStatus(BANK);
            }

            if (typeLabel == "STREET") {
                StreetTile* street = dynamic_cast<StreetTile*>(prop);
                if (street != nullptr) {
                    FestivalState festivalState;
                    festivalState.setStacks(stoi(festivalMult));
                    festivalState.setTurnsLeft(stoi(festivalDur));
                    street->setFestivalState(festivalState);
                    if (nBuilding == "H") {
                        street->setHasHotel(true);
                    } else {
                        street->setHouseCount(stoi(nBuilding));
                    }
                }
            }
        }
    }

    // Load Deck State
    int mainDeckCount = 0;
    in >> mainDeckCount;

    CardDeck<SkillCard>& skillDeck = gameContext.getSkillDeck();
    for (int i = 0; i < mainDeckCount; ++i) {
        string cardName;
        in >> cardName;

        SkillCard* card = nullptr;
        if (cardName == "DiscountCard") {
            int discountPercentage;
            in >> discountPercentage;
            card = new DiscountCard(discountPercentage);
        } else if (cardName == "MoveCard") {
            int steps;
            in >> steps;
            card = new MoveCard(steps);
        } else if (cardName == "ShieldCard") {
            card = new ShieldCard();
        } else if (cardName == "TeleportCard") {
            card = new TeleportCard();
        } else if (cardName == "LassoCard") {
            card = new LassoCard();
        } else if (cardName == "DemolitionCard") {
            card = new DemolitionCard();
        }

        if (card != nullptr) {
            skillDeck.add(card);
        }
    }

    int discardDeckCount = 0;
    in >> discardDeckCount;

    CardDeck<SkillCard>& skillDeckDiscard = gameContext.getSkillDeck();
    for(int i = 0; i < discardDeckCount; i++){
        string cardName;
        in >> cardName;

        SkillCard* card = nullptr;
        if (cardName == "DiscountCard") {
            int discountPercentage;
            in >> discountPercentage;
            card = new DiscountCard(discountPercentage);
        } else if (cardName == "MoveCard") {
            int steps;
            in >> steps;
            card = new MoveCard(steps);
        } else if (cardName == "ShieldCard") {
            card = new ShieldCard();
        } else if (cardName == "TeleportCard") {
            card = new TeleportCard();
        } else if (cardName == "LassoCard") {
            card = new LassoCard();
        } else if (cardName == "DemolitionCard") {
            card = new DemolitionCard();
        }

        if (card != nullptr) {
            skillDeckDiscard.discard(card);
        }
    }

    logger.loadFromStream(in);

    in.close();
    cout << "Game dimuat dari " << fileName << endl;
}

bool SaveLoader::checkSaveExists(string fileName) {
    ifstream in(fileName);
    return in.good();
}