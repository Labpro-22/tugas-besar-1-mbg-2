#include "gui/popups/InputPopup.hpp"

InputPopup::InputPopup(const std::string& prompt) : promptText(prompt), inputText("") {
    popupRect = sf::FloatRect(360.f, 300.f, 400.f, 200.f);
    inputRect = sf::FloatRect(popupRect.left + 20.f, popupRect.top + 80.f, 360.f, 40.f);
    okBtnRect = sf::FloatRect(popupRect.left + 150.f, popupRect.top + 140.f, 100.f, 40.f);
}

void InputPopup::render(sf::RenderWindow& window) {
    draw3DPanel(window, popupRect, sf::Color(192, 192, 192), false);
    draw3DPanel(window, sf::FloatRect(popupRect.left, popupRect.top, popupRect.width, 32.f), sf::Color(40, 80, 200), false);
    
    window.draw(createText("Input Required", popupRect.left + 20.f, popupRect.top + 4.f, 18, sf::Color::White));
    
    std::vector<std::string> wrappedPrompt = wrapTextToWidth(mainFont, 18, promptText, popupRect.width - 40.f);
    float pY = popupRect.top + 45.f;
    for (const auto& line : wrappedPrompt) {
        window.draw(createText(line, popupRect.left + 20.f, pY, 18, sf::Color::Black));
        pY += 22.f;
    }

    draw3DPanel(window, inputRect, sf::Color::White, true);
    
    std::string displayCmd = inputText;
    if (static_cast<int>(cursorClock.getElapsedTime().asSeconds() * 2) % 2 == 0) displayCmd += "_";
    
    window.draw(createText(displayCmd, inputRect.left + 10.f, inputRect.top + 10.f, 18, sf::Color::Black));

    draw3DPanel(window, okBtnRect, sf::Color(200, 200, 200), false);
    window.draw(createText("OK", okBtnRect.left + 35.f, okBtnRect.top + 10.f, 18, sf::Color::Black));
}

PopupResult InputPopup::handleMouseClick(float mouseX, float mouseY) {
    if (containsPoint(okBtnRect, mouseX, mouseY)) {
        return PopupResult::INPUT_SUBMIT;
    }
    return PopupResult::NONE;
}

void InputPopup::handleTextInput(unsigned int unicode) {
    if (unicode == 8) { // Backspace
        if (!inputText.empty()) inputText.pop_back();
    } else if (unicode >= 32 && unicode <= 126 && inputText.size() < 30) {
        inputText += static_cast<char>(unicode);
    }
}

std::string InputPopup::getPopupInputData() {
    return inputText;
}
