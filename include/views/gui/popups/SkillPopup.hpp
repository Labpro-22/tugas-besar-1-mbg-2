#pragma once
#include "gui/popups/Popup.hpp"
#include "SkillCard.hpp"
#include <vector>

class SkillPopup : public Popup {
private:
    std::vector<SkillCard*> cards;
    bool isDiscardMode;
    sf::FloatRect popupRect;
    std::vector<sf::FloatRect> cardRects;
    std::vector<sf::FloatRect> btnRects;
    sf::FloatRect cancelBtnRect;
    int selectedCardIdx;

public:
    SkillPopup(const std::vector<SkillCard*>& playerCards, bool discardMode);
    
    void render(sf::RenderWindow& window) override;
    PopupResult handleMouseClick(float mouseX, float mouseY) override;
    
    int getSelectedCardIndex() const { return selectedCardIdx; }
};
