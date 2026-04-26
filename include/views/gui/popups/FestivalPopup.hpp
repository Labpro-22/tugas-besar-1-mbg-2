#pragma once
#include "gui/popups/Popup.hpp"
#include <vector>
#include <string>

struct FestivalItem {
    int tileIndex;
    std::string name;
    int baseRent;
    int currentMultiplier; // 1, 2, 4, atau 8
    int currentDuration;
};

class FestivalPopup : public Popup {
private:
    std::vector<FestivalItem> items;
    std::vector<sf::FloatRect> actionBtnRects;
    sf::FloatRect closeBtnRect;

    bool showResultState; // Flag untuk mengganti tampilan setelah properti dipilih
    std::string resultMessage;
    int selectedTileIndex;

public:
    FestivalPopup(const std::vector<FestivalItem>& playerProperties);
    
    void render(sf::RenderWindow& window) override;
    PopupResult handleMouseClick(float mouseX, float mouseY) override;
    
    int getSelectedTileIndex() const { return selectedTileIndex; }
};