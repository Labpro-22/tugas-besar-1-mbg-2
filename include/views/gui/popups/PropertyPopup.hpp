#pragma once
#include "gui/popups/Popup.hpp"

class PropertyPopup : public Popup {
private:
    std::string propertyName;
    int price;
    std::string playerName;
    sf::FloatRect buyBtnRect;
    sf::FloatRect auctionBtnRect;

public:
    PropertyPopup(std::string propName, int propPrice, std::string pName);
    void render(sf::RenderWindow& window) override;
    PopupResult handleMouseClick(float mouseX, float mouseY) override;
};