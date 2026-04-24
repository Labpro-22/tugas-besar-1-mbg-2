#include "AuctionController.hpp"

void startAuctionSkipBuy(GameContext &gameContext, DisplayView &dv, InputHandler &inputHandler) {
    Player &skipPlayer = gameContext.getCurrentPlayer();
    int position = gameContext.getCurrentPlayer().getPosition();
    Tile *currentTile = gameContext.getBoard().getTile(position);
    dv.renderAuctionStart(gameContext, dynamic_cast<PropertyTile *>(currentTile));
    
    bool isBid = false;
    vector<pair<Player *, int>> bid;
    int maxBid = 0;
    bool auctionActive = true;
    
    while (auctionActive) {
        auctionActive = false;
        for (auto &i : gameContext.getPlayers()) {
            if (i == skipPlayer) {
                continue;
            } else {
                bool currentBid = false;
                placeBid(i, bid, dv, inputHandler, currentBid, maxBid);
                if (currentBid) {
                    isBid = true;
                    auctionActive = true;
                }
            }
        }
    }

    if (!isBid) {
        cout << "Tidak ada player yang melakukan Bid" << endl;
        cout << "Anda wajib melakukan Bid" << endl;
        while (true) {
            dv.renderAuctionLine(skipPlayer.getName());
            CommandType cmd = inputHandler.getCommand();
            int bidAmount = 0;
            bool hasAmount = false;

            if (cmd == CommandType::BID) {
                inputHandler.getMoneyRemaining(bidAmount, hasAmount);
                if (!hasAmount || bidAmount <= 0) {
                    cout << "Format BID salah. Masukkan jumlah M > 0, contoh: BID M100" << endl;
                } else {
                    dv.renderAuctionResult(skipPlayer.getName(), bidAmount);
                    skipPlayer -= bidAmount;
                    return;
                }
            } else {
                cout << "Hanya bisa melakukan Bid" << endl;
            }
        }
    } else {
        resolveAuction(bid, dv);
    }
}

void startAuctionBankrupt(GameContext &gameContext, DisplayView &dv, InputHandler &inputHandler) {
    Player &bankruptPlayer = gameContext.getCurrentPlayer();
    int position = bankruptPlayer.getPosition();
    Tile *currentTile = gameContext.getBoard().getTile(position);
    dv.renderAuctionStart(gameContext, dynamic_cast<PropertyTile *>(currentTile));
    
    bool isBid = false;
    vector<pair<Player *, int>> bid;
    int maxBid = 0;
    Player *lastEligiblePlayer = nullptr;

    for (auto &p : gameContext.getPlayers()) {
        if (p == bankruptPlayer) {
            continue;
        }
        lastEligiblePlayer = &p;
    }

    if (lastEligiblePlayer == nullptr) {
        return;
    }

    bool auctionActive = true;
    while (auctionActive) {
        auctionActive = false;
        for (auto &i : gameContext.getPlayers()) {
            if (i == bankruptPlayer) {
                continue;
            } else {
                bool currentBid = false;
                placeBid(i, bid, dv, inputHandler, currentBid, maxBid);
                if (currentBid) {
                    isBid = true;
                    auctionActive = true;
                }
            }
        }
    }

    if (!isBid) {
        cout << "Tidak ada player yang melakukan Bid" << endl;
        cout << "Anda wajib melakukan Bid, Player : " << lastEligiblePlayer->getName() << endl;
        while (true) {
            dv.renderAuctionLine(lastEligiblePlayer->getName());
            CommandType cmd = inputHandler.getCommand();
            int bidAmount = 0;
            bool hasAmount = false;

            if (cmd == CommandType::BID) {
                inputHandler.getMoneyRemaining(bidAmount, hasAmount);
                if (!hasAmount || bidAmount <= 0) {
                    cout << "Format BID salah. Masukkan jumlah M > 0, contoh: BID M100" << endl;
                } else {
                    dv.renderAuctionResult(lastEligiblePlayer->getName(), bidAmount);
                    *lastEligiblePlayer -= bidAmount;
                    return;
                }
            } else {
                cout << "Hanya bisa melakukan Bid" << endl;
            }
        }
    } else {
        resolveAuction(bid, dv);
    }
}

void placeBid(Player &bidder, vector<pair<Player *, int>> &bid, DisplayView &dv, InputHandler &inputHandler, bool &isBid, int &maxBid) {
    dv.renderAuctionLine(bidder.getName());
    int bidAmount = 0;
    bool hasAmount = false;
    
    while (true) {
        dv.renderAuctionLine(bidder.getName());
        CommandType cmd = inputHandler.getCommand();
        if (cmd == CommandType::PASS) {
            return;
        } else if (cmd == CommandType::BID) {
            inputHandler.getMoneyRemaining(bidAmount, hasAmount);
            if (bidder.getBalance() < bidAmount) {
                cout << "BID anda lebih besar dari balance" << endl;
            } else if (!hasAmount || bidAmount <= maxBid) {
                cout << "Format BID salah atau bid tidak lebih besar dari " << maxBid << endl;
            } else {
                dv.HighestBidder(bidder.getName(), bidAmount);
                maxBid = bidAmount;
                bid.push_back(make_pair(&bidder, bidAmount));
                isBid = true;
                return;
            }
        } else {
            cout << "Perintah tidak dikenali. Silakan lakukan BID." << endl;
        }
    }
}

void resolveAuction(vector<pair<Player *, int>> &bid, DisplayView &dv) {
    if (bid.empty()) {
        return;
    }
    
    Player *maxBidder = bid.at(0).first;
    int maxNow = bid.at(0).second;

    for (auto &i : bid) {
        if (i.second > maxNow) {
            maxBidder = i.first;
            maxNow = i.second;
        }
    }
    
    dv.renderAuctionResult(maxBidder->getName(), maxNow);
    *maxBidder -= maxNow;
}