#pragma once
#include "PropertyTile.hpp"
#include "Player.hpp"
#include "GameContext.hpp"
#include "DisplayView.hpp"
#include "InputHandler.hpp"
#include <list>
using namespace std;

class AuctionController 
{
public:
    void startAuctionSkipBuy(GameContext &gameContext, DisplayView &dv, InputHandler &inputHandler);
    void startAuctionBankrupt(GameContext &gameContext, DisplayView &dv, InputHandler &inputHandler, PropertyTile* property);
    void runAuctionLogic(GameContext &gameContext, DisplayView &dv, InputHandler &inputHandler, vector<Player*> participants, int startIndex, PropertyTile* property);
    void placeBid(Player &bidder, vector<pair<Player *, int>> &bidHistory, DisplayView &dv, InputHandler &inputHandler, int &maxBid, bool &currentBidSuccess, int &passCount, bool forceBidMode);
    void resolveAuction(vector<pair<Player *, int>> &bid, DisplayView &dv, PropertyTile* property);
};
