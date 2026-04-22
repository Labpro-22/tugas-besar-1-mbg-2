#pragma once
#include <list>
#include "Player.hpp"
#include "GameContext.hpp"
#include "EffectController.hpp"
#include "EconomyController.hpp"
#include "AuctionController.hpp"
#include "BankruptcyController.hpp"

using namespace std;

enum class Command {
    LEMPAR_DADU,
    ATUR_DADU,
    CETAK_PAPAN,
    CETAK_AKTA,
    CETAK_PROPERTI,
    GADAI,
    TEBUS,
    BANGUN,
    GUNAKAN_KEMAMPUAN,
    SIMPAN,
    MUAT,
    CETAK_LOG,
    UNKNOWN_COMMAND
};

class TurnController
{
private: 
    Command parseCommand(const std::string& cmdInput);
public:
    bool checkWinCondition();
    void executeAction(GameContext* context, EconomyController& eco, EffectController& eff, AuctionController& auc, BankruptcyController& bank);
};