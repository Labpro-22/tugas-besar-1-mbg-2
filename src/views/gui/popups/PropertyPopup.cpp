#include "gui/popups/PropertyPopup.hpp"

PropertyPopup::PropertyPopup(std::string propName, int propPrice, std::string pName) 
    : propertyName(propName), price(propPrice), playerName(pName) {
    buyBtnRect = sf::FloatRect(popupRect.left + 220.f, popupRect.top + 260.f, 110.f, 50.f);
    auctionBtnRect = sf::FloatRect(popupRect.left + 350.f, popupRect.top + 260.f, 130.f, 50.f);
}

void PropertyPopup::render(sf::RenderWindow& window) {
    draw3DPanel(window, popupRect, sf::Color(192, 192, 192), false);
    
    draw3DPanel(window, sf::FloatRect(popupRect.left, popupRect.top, popupRect.width, 32.f), sf::Color(40, 80, 200), false);
    window.draw(createText("Buy", popupRect.left + (popupRect.width/2.f) - 20.f, popupRect.top + 4.f, 18, sf::Color::White));

    window.draw(createText(playerName + ",", popupRect.left + 20.f, popupRect.top + 50.f, 22, sf::Color::Black));
    window.draw(createText("You may purchase this property or", popupRect.left + 40.f, popupRect.top + 85.f, 18, sf::Color::Black));
    window.draw(createText("allow it to go up for auction.", popupRect.left + 30.f, popupRect.top + 115.f, 18, sf::Color::Black));

    window.draw(createText(propertyName, popupRect.left + 240.f, popupRect.top + 175.f, 20, sf::Color::Black));
    window.draw(createText("Price: $" + std::to_string(price), popupRect.left + 260.f, popupRect.top + 205.f, 18, sf::Color::Black));

    draw3DPanel(window, buyBtnRect, sf::Color(200, 200, 200), false);
    window.draw(createText("Buy It", buyBtnRect.left + 22.f, buyBtnRect.top + 12.f, 18, sf::Color::Black));

    draw3DPanel(window, auctionBtnRect, sf::Color(200, 200, 200), false);
    window.draw(createText("Auction", auctionBtnRect.left + 20.f, auctionBtnRect.top + 12.f, 18, sf::Color::Black));
}

PopupResult PropertyPopup::handleMouseClick(float mouseX, float mouseY) {
    if (containsPoint(buyBtnRect, mouseX, mouseY)) return PopupResult::BUY_PROPERTY;
    if (containsPoint(auctionBtnRect, mouseX, mouseY)) return PopupResult::AUCTION_PROPERTY;
    return PopupResult::NONE;
}