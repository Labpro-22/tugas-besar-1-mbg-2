#include "gui/popups/MortgagePopup.hpp"

MortgagePopup::MortgagePopup(const std::vector<MortgageItem>& playerProperties) 
    : items(playerProperties), clickedItemIndex(-1) {
    
    float height = 120.f + (items.size() * 50.f);
    popupRect = sf::FloatRect(260.f, 150.f, 600.f, height);
    
    for (size_t i = 0; i < items.size(); ++i) {
        float yPos = popupRect.top + 50.f + (i * 50.f);
        actionBtnRects.push_back(sf::FloatRect(popupRect.left + 450.f, yPos, 120.f, 35.f));
    }

    closeBtnRect = sf::FloatRect(popupRect.left + 250.f, popupRect.top + height - 50.f, 100.f, 40.f);
}

void MortgagePopup::render(sf::RenderWindow& window) {
    draw3DPanel(window, popupRect, sf::Color(220, 220, 220), false);
    
    draw3DPanel(window, sf::FloatRect(popupRect.left, popupRect.top, popupRect.width, 35.f), sf::Color(40, 80, 200), false);
    window.draw(createText("=== Properties Available for Mortgage ===", popupRect.left + 100.f, popupRect.top + 6.f, 18, sf::Color::White));

    for (size_t i = 0; i < items.size(); ++i) {
        float yPos = popupRect.top + 50.f + (i * 50.f);
        
        std::string infoText = std::to_string(i + 1) + ". " + items[i].name + " [" + items[i].colorGroup + "] Value: M" + std::to_string(items[i].mortgageValue);
        window.draw(createText(infoText, popupRect.left + 20.f, yPos + 5.f, 16, sf::Color::Black));

        if (items[i].hasBuildings) {
            draw3DPanel(window, actionBtnRects[i], sf::Color(255, 165, 0), false); 
            window.draw(createText("Sell Build.", actionBtnRects[i].left + 15.f, actionBtnRects[i].top + 8.f, 14, sf::Color::Black));
        } else {
            draw3DPanel(window, actionBtnRects[i], sf::Color(200, 200, 200), false); 
            window.draw(createText("MORTGAGE", actionBtnRects[i].left + 22.f, actionBtnRects[i].top + 8.f, 14, sf::Color::Black));
        }
    }

    if (items.empty()) {
        window.draw(createText("No properties can be mortgaged at this time.", popupRect.left + 100.f, popupRect.top + 70.f, 18, sf::Color::Red));
    }

    draw3DPanel(window, closeBtnRect, sf::Color(200, 200, 200), false);
    window.draw(createText("Close", closeBtnRect.left + 25.f, closeBtnRect.top + 8.f, 18, sf::Color::Black));
}

PopupResult MortgagePopup::handleMouseClick(float mouseX, float mouseY) {
    if (containsPoint(closeBtnRect, mouseX, mouseY)) {
        return PopupResult::CLOSE_POPUP;
    }

    for (size_t i = 0; i < actionBtnRects.size(); ++i) {
        if (containsPoint(actionBtnRects[i], mouseX, mouseY)) {
            clickedItemIndex = items[i].tileIndex; 
            if (items[i].hasBuildings) {
                return PopupResult::SELL_BUILDINGS;
            } else {
                return PopupResult::MORTGAGE_PROPERTY;
            }
        }
    }
    return PopupResult::NONE;
}

int MortgagePopup::getSelectedTileIndex() const {
    return clickedItemIndex;
}