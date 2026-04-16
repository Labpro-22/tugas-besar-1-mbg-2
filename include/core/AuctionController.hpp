#pragma once
#include "PropertyTile.hpp"
#include "Player.hpp"
#include <list>
using namespace std;

class AuctionController 
{
private:
    PropertyTile* auctionItem;
    int highestBid;
    Player* highestBidder;
    list<Player*> activeBidders;
public:
    void startAuction(PropertyTile* item, list<Player*> bidders);
    void placeBid(Player* bidder, int bidAmount);
    void resolveAuction();
};

