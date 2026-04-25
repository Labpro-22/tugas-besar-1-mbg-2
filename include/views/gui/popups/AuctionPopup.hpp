#pragma once
#include "gui/popups/Popup.hpp"
#include <vector>
#include <array>

class AuctionPopup : public Popup {
private:
    std::string propertyName;
    int currentBid;
    int highBidderIdx;
    int activeBidderIdx; // Giliran bid player
    
    sf::FloatRect bidBtnRect;
    sf::FloatRect passBtnRect;

public:
    AuctionPopup(std::string propName, int startBid, int starterIdx);
    void render(sf::RenderWindow& window) override;
    PopupResult handleMouseClick(float mouseX, float mouseY) override;
    void updateBid(int amount, int playerIdx);
    void nextTurn(const std::array<bool, 4>& activePlayers);

    // Getter
    int getCurrentBid() const { return currentBid; }
    int getHighBidderIdx() const { return highBidderIdx; }
    int getActiveBidderIdx() const { return activeBidderIdx; }
};