#include "gui/popups/CardPopup.hpp"

CardPopup::CardPopup(std::string type, std::string desc, std::string buttonText) 
    : cardType(type), description(desc), btnText(buttonText) {
    
    popupRect = sf::FloatRect(310.f, 220.f, 500.f, 300.f); 
    
    if (cardType == "Dana Umum") {
        bgColor = sf::Color(255, 235, 59); // Kuning = Dana Umum
    } else {
        bgColor = sf::Color(255, 152, 0); // Oranye = Kesempatan
    }

    okBtnRect = sf::FloatRect(popupRect.left + 200.f, popupRect.top + 230.f, 100.f, 40.f);
}

void CardPopup::render(sf::RenderWindow& window) {
    draw3DPanel(window, popupRect, sf::Color(192, 192, 192), false);
    
    sf::FloatRect innerRect(popupRect.left + 10.f, popupRect.top + 10.f, popupRect.width - 20.f, 200.f);
    draw3DPanel(window, innerRect, bgColor, true); 

    sf::Text titleText = createText(cardType, innerRect.left + 15.f, innerRect.top + 10.f, 24, sf::Color::Black);
    titleText.setStyle(sf::Text::Italic); 
    window.draw(titleText);

    window.draw(createText(description, innerRect.left + 40.f, innerRect.top + 80.f, 22, sf::Color::Black));

    draw3DPanel(window, okBtnRect, sf::Color(200, 200, 200), false);
    window.draw(createText(btnText, okBtnRect.left + (okBtnRect.width/2.f) - 15.f, okBtnRect.top + 8.f, 18, sf::Color::Black));
}

PopupResult CardPopup::handleMouseClick(float mouseX, float mouseY) {
    if (containsPoint(okBtnRect, mouseX, mouseY)) return PopupResult::OK_CLOSE;
    return PopupResult::NONE;
}