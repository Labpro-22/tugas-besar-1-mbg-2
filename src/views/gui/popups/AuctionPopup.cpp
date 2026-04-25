#include "gui/popups/AuctionPopup.hpp"

AuctionPopup::AuctionPopup(std::string propName, int startBid, int starterIdx)
    : propertyName(propName), currentBid(startBid), highBidderIdx(-1), activeBidderIdx(starterIdx) {
    
    popupRect = sf::FloatRect(310.f, 200.f, 500.f, 350.f);
    bidBtnRect = sf::FloatRect(popupRect.left + 120.f, popupRect.top + 260.f, 100.f, 50.f);
    passBtnRect = sf::FloatRect(popupRect.left + 280.f, popupRect.top + 260.f, 100.f, 50.f);
}

void AuctionPopup::render(sf::RenderWindow& window) {
    draw3DPanel(window, popupRect, sf::Color(192, 192, 192), false);
    
    draw3DPanel(window, sf::FloatRect(popupRect.left, popupRect.top, popupRect.width, 32.f), sf::Color(34, 139, 34), false);
    window.draw(createText("Lelang Properti: " + propertyName, popupRect.left + 20.f, popupRect.top + 4.f, 18, sf::Color::White));

    std::string bidderName = (highBidderIdx == -1) ? "Belum ada" : "Pemain " + std::to_string(highBidderIdx + 1);
    window.draw(createText("Bid Tertinggi: $" + std::to_string(currentBid), popupRect.left + 50.f, popupRect.top + 60.f, 22, sf::Color::Black));
    window.draw(createText("Oleh: " + bidderName, popupRect.left + 50.f, popupRect.top + 90.f, 18, sf::Color(80, 80, 80)));

    draw3DPanel(window, sf::FloatRect(popupRect.left + 50.f, popupRect.top + 140.f, 400.f, 80.f), sf::Color(210, 210, 210), true);
    window.draw(createText("Giliran: Pemain " + std::to_string(activeBidderIdx + 1), popupRect.left + 70.f, popupRect.top + 155.f, 20, sf::Color::Red));
    
    int nextBid = (currentBid == 0) ? 0 : currentBid + 10;
    window.draw(createText("Bid minimum: $" + std::to_string(nextBid), popupRect.left + 70.f, popupRect.top + 185.f, 16, sf::Color::Black));

    draw3DPanel(window, bidBtnRect, sf::Color(200, 200, 200), false);
    window.draw(createText("BID +$10", bidBtnRect.left + 15.f, bidBtnRect.top + 12.f, 18, sf::Color::Black));

    draw3DPanel(window, passBtnRect, sf::Color(200, 200, 200), false);
    window.draw(createText("PASS", passBtnRect.left + 25.f, passBtnRect.top + 12.f, 18, sf::Color::Black));
}

PopupResult AuctionPopup::handleMouseClick(float mouseX, float mouseY) {
    if (containsPoint(bidBtnRect, mouseX, mouseY)) return PopupResult::AUCTION_BID;
    if (containsPoint(passBtnRect, mouseX, mouseY)) return PopupResult::AUCTION_PASS;
    return PopupResult::NONE;
}

void AuctionPopup::updateBid(int amount, int playerIdx) {
    currentBid = amount;
    highBidderIdx = playerIdx;
}

void AuctionPopup::nextTurn(const std::array<bool, 4>& activePlayers) {
    // Cari player aktif berikutnya
    do {
        activeBidderIdx = (activeBidderIdx + 1) % 4;
    } while (!activePlayers[activeBidderIdx]);
}