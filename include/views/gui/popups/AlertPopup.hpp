#pragma once
#include "gui/popups/Popup.hpp"

class AlertPopup : public Popup {
private:
    std::string title;
    std::string message;
    std::string playerName;
    
    std::string btn1Text;
    std::string btn2Text;

    sf::FloatRect btn1Rect;
    sf::FloatRect btn2Rect;

public:
    AlertPopup(std::string popupTitle, std::string msg, std::string pName, std::string b1Text, std::string b2Text = "");
    
    void render(sf::RenderWindow& window) override;
    PopupResult handleMouseClick(float mouseX, float mouseY) override;
};