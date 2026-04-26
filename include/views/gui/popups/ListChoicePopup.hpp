#pragma once
#include "gui/popups/Popup.hpp"
#include <vector>
#include <string>

class ListChoicePopup : public Popup {
private:
    std::string promptText;
    std::vector<std::string> options;
    std::vector<sf::FloatRect> optionRects;
    sf::FloatRect popupRect;
    sf::FloatRect cancelBtnRect;
    int selectedIdx;

public:
    ListChoicePopup(const std::string& prompt, const std::vector<std::string>& opts);
    
    void render(sf::RenderWindow& window) override;
    PopupResult handleMouseClick(float mouseX, float mouseY) override;
    
    int getSelectedIndex() const { return selectedIdx; }
};
