#pragma once
#include "gui/popups/Popup.hpp"
#include <vector>
#include <string>

struct MortgageItem {
    int tileIndex;
    std::string name;
    std::string colorGroup;
    int mortgageValue;
    bool hasBuildings; 
};

class MortgagePopup : public Popup {
private:
    std::vector<MortgageItem> items;
    std::vector<sf::FloatRect> actionBtnRects;
    sf::FloatRect closeBtnRect;

    int clickedItemIndex; 

public:
    MortgagePopup(const std::vector<MortgageItem>& playerProperties);
    
    void render(sf::RenderWindow& window) override;
    PopupResult handleMouseClick(float mouseX, float mouseY) override;
    
    // Untuk diambil oleh SFMLView setelah sinyal dikembalikan
    int getSelectedTileIndex() const; 
};