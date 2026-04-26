#include "gui/popups/AlertPopup.hpp"

AlertPopup::AlertPopup(std::string popupTitle, std::string msg, std::string pName, std::string b1Text, std::string b2Text)
    : title(popupTitle), message(msg), playerName(pName), btn1Text(b1Text), btn2Text(b2Text) {
    
    popupRect = sf::FloatRect(310.f, 220.f, 500.f, 320.f);

    if (!btn2Text.empty()) {
        btn1Rect = sf::FloatRect(popupRect.left + 120.f, popupRect.top + 250.f, 100.f, 40.f);
        btn2Rect = sf::FloatRect(popupRect.left + 280.f, popupRect.top + 250.f, 100.f, 40.f);
    } else {
        btn1Rect = sf::FloatRect(popupRect.left + 200.f, popupRect.top + 250.f, 100.f, 40.f);
    }
}

void AlertPopup::render(sf::RenderWindow& window) {
    draw3DPanel(window, popupRect, sf::Color(192, 192, 192), false);
    
    draw3DPanel(window, sf::FloatRect(popupRect.left, popupRect.top, popupRect.width, 32.f), sf::Color(40, 80, 200), false);
    window.draw(createText(title, popupRect.left + (popupRect.width/2.f) - 40.f, popupRect.top + 4.f, 18, sf::Color::White));

    window.draw(createText(playerName, popupRect.left + (popupRect.width/2.f) - 40.f, popupRect.top + 80.f, 20, sf::Color::Black));

    std::vector<std::string> wrapped = wrapTextToWidth(mainFont, 18, message, popupRect.width - 60.f);
    float textY = popupRect.top + 120.f;
    for (const auto& line : wrapped) {
        window.draw(createText(line, popupRect.left + 30.f, textY, 18, sf::Color::Black));
        textY += 22.f;
        if (textY > popupRect.top + 240.f) break; 
    }

    draw3DPanel(window, btn1Rect, sf::Color(200, 200, 200), false);
    window.draw(createText(btn1Text, btn1Rect.left + 20.f, btn1Rect.top + 8.f, 18, sf::Color::Black));

    if (!btn2Text.empty()) {
        draw3DPanel(window, btn2Rect, sf::Color(200, 200, 200), false);
        window.draw(createText(btn2Text, btn2Rect.left + 20.f, btn2Rect.top + 8.f, 18, sf::Color::Black));
    }
}

PopupResult AlertPopup::handleMouseClick(float mouseX, float mouseY) {
    if (containsPoint(btn1Rect, mouseX, mouseY)) {
        return btn2Text.empty() ? PopupResult::OK_CLOSE : PopupResult::TAX_PERCENTAGE;
    }
    if (!btn2Text.empty() && containsPoint(btn2Rect, mouseX, mouseY)) {
        return PopupResult::TAX_FLAT;
    }
    return PopupResult::NONE;
}