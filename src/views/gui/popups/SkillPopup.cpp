#include "gui/popups/SkillPopup.hpp"
#include <sstream>

SkillPopup::SkillPopup(const std::vector<SkillCard*>& playerCards, bool discardMode)
    : cards(playerCards), isDiscardMode(discardMode), selectedCardIdx(-1) {
    
    popupRect = sf::FloatRect(200.f, 150.f, 800.f, 450.f);
    
    float cardWidth = 180.f;
    float cardHeight = 250.f;
    float spacing = 20.f;
    
    int numCards = cards.size();
    float totalWidth = numCards * cardWidth + (numCards - 1) * spacing;
    float startX = popupRect.left + (popupRect.width - totalWidth) / 2.f;
    float startY = popupRect.top + 100.f;
    
    for (int i = 0; i < numCards; ++i) {
        cardRects.push_back(sf::FloatRect(startX + i * (cardWidth + spacing), startY, cardWidth, cardHeight));
        btnRects.push_back(sf::FloatRect(startX + i * (cardWidth + spacing) + 40.f, startY + cardHeight - 40.f, 100.f, 30.f));
    }
    
    cancelBtnRect = sf::FloatRect(popupRect.left + popupRect.width / 2.f - 50.f, popupRect.top + popupRect.height - 50.f, 100.f, 35.f);
}

void SkillPopup::render(sf::RenderWindow& window) {
    draw3DPanel(window, popupRect, sf::Color(192, 192, 192), false);
    
    draw3DPanel(window, sf::FloatRect(popupRect.left, popupRect.top, popupRect.width, 32.f), sf::Color(0, 51, 102), false);
    std::string title = isDiscardMode ? "Discard a Skill Card" : "Your Skill Cards";
    window.draw(createText(title, popupRect.left + 20.f, popupRect.top + 4.f, 18, sf::Color::White));
    
    if (cards.empty()) {
        window.draw(createText("You have no Skill Cards.", popupRect.left + 250.f, popupRect.top + 200.f, 20, sf::Color::Black));
    }

    for (int i = 0; i < (int)cards.size(); ++i) {
        draw3DPanel(window, cardRects[i], sf::Color(255, 255, 240), true);
        
        window.draw(createText(cards[i]->getName(), cardRects[i].left + 10.f, cardRects[i].top + 10.f, 16, sf::Color(0, 0, 150)));
        
        std::string desc = cards[i]->getDescription();
        std::vector<std::string> wrapped = wrapTextToWidth(mainFont, 14, desc, cardRects[i].width - 20.f);
        float descY = cardRects[i].top + 40.f;
        for (const auto& line : wrapped) {
            window.draw(createText(line, cardRects[i].left + 10.f, descY, 14, sf::Color::Black));
            descY += 16.f;
            if (descY > btnRects[i].top - 5.f) break; 
        }
        
        draw3DPanel(window, btnRects[i], isDiscardMode ? sf::Color(200, 50, 50) : sf::Color(50, 200, 50), false);
        std::string btnText = isDiscardMode ? "DISCARD" : "USE";
        window.draw(createText(btnText, btnRects[i].left + (isDiscardMode ? 15.f : 32.f), btnRects[i].top + 5.f, 16, sf::Color::White));
    }

    draw3DPanel(window, cancelBtnRect, sf::Color(150, 150, 150), false);
    window.draw(createText("CANCEL", cancelBtnRect.left + 18.f, cancelBtnRect.top + 7.f, 16, sf::Color::White));
}

PopupResult SkillPopup::handleMouseClick(float mouseX, float mouseY) {
    if (containsPoint(cancelBtnRect, mouseX, mouseY)) {
        selectedCardIdx = -1;
        return PopupResult::INPUT_SUBMIT;
    }

    for (int i = 0; i < (int)btnRects.size(); ++i) {
        if (containsPoint(btnRects[i], mouseX, mouseY)) {
            selectedCardIdx = i;
            return PopupResult::INPUT_SUBMIT; 
        }
    }
    
    if (!containsPoint(popupRect, mouseX, mouseY)) {
        selectedCardIdx = -1;
        return PopupResult::INPUT_SUBMIT; 
    }
    return PopupResult::NONE;
}
