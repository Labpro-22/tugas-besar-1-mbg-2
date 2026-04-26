#pragma once
#include "gui/popups/Popup.hpp"

class CardPopup : public Popup {
private:
    std::string cardType; // "Kesempatan" atau "Dana Umum"
    std::string description;
    std::string btnText;
    sf::Color bgColor;
    
    sf::FloatRect okBtnRect;

public:
    // cardType menentukan warna background.
    CardPopup(std::string type, std::string desc, std::string buttonText = "Ok");
    
    void render(sf::RenderWindow& window) override;
    PopupResult handleMouseClick(float mouseX, float mouseY) override;
};