#include "AuctionController.hpp"
#include "GameLogger.hpp"

void AuctionController::startAuctionSkipBuy(GameContext &gameContext, DisplayView &dv, InputHandler &inputHandler, GameLogger &logger) {
    PropertyTile* property = dynamic_cast<PropertyTile*>(gameContext.getBoard().getTile(gameContext.getCurrentPlayer().getPosition()));
    
    vector<Player*> participants;
    for (auto &p : gameContext.getPlayers()) {
        if (p.getStatus() != PlayerStatus::BANKRUPT) {
            participants.push_back(&p);
        }
    }

    int startIndexInGlobal = (gameContext.getCurrentPlayerIndex() + 1) % gameContext.getPlayers().size();
    
    int startIdx = 0;
    for(int i = 0; i < participants.size(); i++) {
        if (participants[i]->getName() == gameContext.getPlayers()[startIndexInGlobal].getName()) {
            startIdx = i;
            break;
        }
    }

    dv.renderAuctionStart(gameContext, property);
    runAuctionLogic(gameContext, dv, inputHandler, participants, startIdx, property, logger);
}

void AuctionController::startAuctionBankrupt(GameContext &gameContext, DisplayView &dv, InputHandler &inputHandler, PropertyTile* property, GameLogger &logger) {
    Player &bankruptPlayer = gameContext.getCurrentPlayer();
    
    vector<Player*> participants;
    for (auto &p : gameContext.getPlayers()) {
        if (p.getName() != bankruptPlayer.getName() && p.getStatus() != PlayerStatus::BANKRUPT) {
            participants.push_back(&p);
        }
    }

    if (participants.size() <= 1) return;

    dv.renderAuctionStart(gameContext, property);
    runAuctionLogic(gameContext, dv, inputHandler, participants, 0, property, logger);
}

void AuctionController::runAuctionLogic(GameContext &gameContext, DisplayView &dv, InputHandler &inputHandler, vector<Player*> participants, int startIndex, PropertyTile* property, GameLogger &logger) {
    int maxBid = -1; 
    int passCount = 0;
    int currentIndex = startIndex;
    int numParticipants = participants.size();
    vector<pair<Player*, int>> bidHistory;

    while (true) {
        Player* currentBidder = participants[currentIndex];
        bool currentBidSuccess = false;

        Player* lastBidder = bidHistory.empty() ? nullptr : bidHistory.back().first;
        bool forceBidMode = (lastBidder == nullptr && passCount == numParticipants - 1);

        placeBid(gameContext, *currentBidder, bidHistory, dv, inputHandler, maxBid, currentBidSuccess, passCount, forceBidMode, property, logger);

        if (maxBid != -1 && passCount == numParticipants - 1) {
            break;
        }

        currentIndex = (currentIndex + 1) % numParticipants;
    }

    if (!bidHistory.empty()) {
        resolveAuction(gameContext, bidHistory, dv, property, logger);
    }
}

void AuctionController::placeBid(GameContext &gameContext, Player &bidder, vector<pair<Player *, int>> &bidHistory, DisplayView &dv, InputHandler &inputHandler, int &maxBid, bool &currentBidSuccess, int &passCount, bool forceBidMode, PropertyTile* property, GameLogger &logger) {
    int bidAmount = 0;
    bool hasAmount = false;
    currentBidSuccess = false;
    while (true) {
        dv.renderAuctionLine(bidder.getName());
        CommandType cmd = inputHandler.getCommand();

        if (cmd == CommandType::PASS) {
            if (forceBidMode) {
                dv.renderPrompt("You must place a bid since everyone else has passed.\n") ;
                continue;
            }
            passCount++;
            return;
        }
        if (cmd == CommandType::BID) {
            inputHandler.getMoneyRemaining(bidAmount, hasAmount);

            if (!hasAmount) {
                dv.renderPrompt("Please enter a valid bid amount.");
                continue;
            }
            if (bidder.getBalance() < bidAmount) {
                dv.renderPrompt("You don't have enough balance to place that bid.");
                continue;
            }

            bool isFirstBidValid = (maxBid == -1 && bidAmount >= 0);
            bool isHigherBidValid = (maxBid != -1 && bidAmount > maxBid);

            if (isFirstBidValid || isHigherBidValid) {
                maxBid = bidAmount;
                bidHistory.push_back({&bidder, bidAmount});
                currentBidSuccess = true;
                passCount = 0; 
                dv.HighestBidder(bidder.getName(), bidAmount);
                logger.addLog(gameContext.getCurrentTurn(), bidder.getName(), "AUCTION_BID", property->getName() + " bid M" + to_string(bidAmount));
                return;
            } else {
                dv.renderPrompt("Bid must be higher than the current maximum bid of M" + to_string(maxBid == -1 ? 0 : maxBid));
            }
        } else {
            dv.renderPrompt("Only BID or PASS commands are accepted.");
        }
    }
}

void AuctionController::resolveAuction(GameContext &gameContext, vector<pair<Player *, int>> &bid, DisplayView &dv, PropertyTile* property, GameLogger &logger) {
    if (bid.empty()) {
        return;
    }
    
    Player* winner = bid.back().first;
    int finalPrice = bid.back().second;

    dv.renderAuctionResult(winner->getName(), finalPrice);
    *winner -= finalPrice;
    property->setOwner(winner);

    if (property->getStatus() == BANK) {
        property->setStatus(OWNED);
    }

    winner->addProperty(property);
    logger.addLog(gameContext.getCurrentTurn(), winner->getName(), "AUCTION_RESULT", property->getName() + " acquired for M" + to_string(finalPrice));
}