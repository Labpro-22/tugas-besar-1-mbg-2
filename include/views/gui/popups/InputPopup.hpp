#pragma once
#include "gui/popups/Popup.hpp"
#include <string>

class InputPopup : public Popup {
private:
    std::string promptText;
    std::string inputText;
    sf::FloatRect inputRect;
    sf::FloatRect okBtnRect;
    sf::Clock cursorClock;

public:
    InputPopup(const std::string& prompt);

    void render(sf::RenderWindow& window) override;
    PopupResult handleMouseClick(float mouseX, float mouseY) override;
    void handleTextInput(unsigned int unicode) override;
    std::string getPopupInputData() override;
};
