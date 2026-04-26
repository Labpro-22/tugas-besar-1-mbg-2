#include "gui/popups/FestivalPopup.hpp"

FestivalPopup::FestivalPopup(const std::vector<FestivalItem>& playerProperties) 
    : items(playerProperties), showResultState(false), selectedTileIndex(-1) {
    
    float height = std::max(250.f, 100.f + (items.size() * 45.f));
    popupRect = sf::FloatRect(280.f, 200.f, 540.f, height);
    
    for (size_t i = 0; i < items.size(); ++i) {
        float yPos = popupRect.top + 80.f + (i * 45.f);
        actionBtnRects.push_back(sf::FloatRect(popupRect.left + 420.f, yPos, 90.f, 30.f));
    }

    closeBtnRect = sf::FloatRect(popupRect.left + 220.f, popupRect.top + height - 50.f, 100.f, 35.f);
}

void FestivalPopup::render(sf::RenderWindow& window) {
    draw3DPanel(window, popupRect, sf::Color(220, 220, 220), false);
    
    draw3DPanel(window, sf::FloatRect(popupRect.left, popupRect.top, popupRect.width, 35.f), sf::Color(138, 43, 226), false);
    window.draw(createText("=== FESTIVAL ===", popupRect.left + 180.f, popupRect.top + 6.f, 18, sf::Color::White));

    if (!showResultState) {
        window.draw(createText("You landed on the Festival tile!", popupRect.left + 20.f, popupRect.top + 45.f, 18, sf::Color::Black));
        
        for (size_t i = 0; i < items.size(); ++i) {
            float yPos = popupRect.top + 80.f + (i * 45.f);
            
            std::string info = std::to_string(i + 1) + ". " + items[i].name + " (Rent: M" + std::to_string(items[i].baseRent * items[i].currentMultiplier) + ")";
            window.draw(createText(info, popupRect.left + 20.f, yPos + 5.f, 16, sf::Color::Black));

            draw3DPanel(window, actionBtnRects[i], sf::Color(200, 200, 200), false);
            window.draw(createText("SELECT", actionBtnRects[i].left + 15.f, actionBtnRects[i].top + 5.f, 14, sf::Color::Black));
        }

        if (items.empty()) {
            window.draw(createText("You don't own any properties yet.", popupRect.left + 20.f, popupRect.top + 100.f, 16, sf::Color::Red));
            draw3DPanel(window, closeBtnRect, sf::Color(200, 200, 200), false);
            window.draw(createText("Close", closeBtnRect.left + 25.f, closeBtnRect.top + 8.f, 16, sf::Color::Black));
        }
    } else {
        window.draw(createText(resultMessage, popupRect.left + 30.f, popupRect.top + 60.f, 18, sf::Color::Black));
        
        draw3DPanel(window, closeBtnRect, sf::Color(200, 200, 200), false);
        window.draw(createText("OK", closeBtnRect.left + 35.f, closeBtnRect.top + 8.f, 18, sf::Color::Black));
    }
}

PopupResult FestivalPopup::handleMouseClick(float mouseX, float mouseY) {
    if (!showResultState) {
        if (items.empty() && containsPoint(closeBtnRect, mouseX, mouseY)) {
            return PopupResult::CLOSE_POPUP;
        }

        for (size_t i = 0; i < actionBtnRects.size(); ++i) {
            if (containsPoint(actionBtnRects[i], mouseX, mouseY)) {
                selectedTileIndex = items[i].tileIndex;
                
                int currentSewa = items[i].baseRent * items[i].currentMultiplier;
                
                if (items[i].currentMultiplier >= 8) {
                    resultMessage = "Effect is at maximum!\n(Rent already doubled 3 times)\n\nDuration reset to: 3 turns";
                } 
                else if (items[i].currentMultiplier > 1) {
                    int newSewa = currentSewa * 2;
                    resultMessage = "Effect strengthened!\n\nPrevious Rent: M" + std::to_string(currentSewa) + "\nNew Rent: M" + std::to_string(newSewa) + "\nDuration reset to: 3 turns";
                } 
                else {
                    int newSewa = currentSewa * 2;
                    resultMessage = "Festival effect active!\n\nInitial Rent: M" + std::to_string(currentSewa) + "\nNew Rent: M" + std::to_string(newSewa) + "\nDuration: 3 turns";
                }
                
                showResultState = true; 
                return PopupResult::NONE; 
            }
        }
    } else {
        if (containsPoint(closeBtnRect, mouseX, mouseY)) {
            return PopupResult::APPLY_FESTIVAL;
        }
    }
    return PopupResult::NONE;
}