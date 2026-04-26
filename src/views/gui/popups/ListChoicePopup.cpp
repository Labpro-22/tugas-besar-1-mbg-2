#include "gui/popups/ListChoicePopup.hpp"

ListChoicePopup::ListChoicePopup(const std::string& prompt, const std::vector<std::string>& opts)
    : promptText(prompt), options(opts), selectedIdx(-1) {

    float popW = 500.f;
    float rowH = 36.f;
    float headerH = 60.f;
    float cancelH = 44.f;
    float padding = 10.f;
    float popH = headerH + options.size() * (rowH + padding) + cancelH + 20.f;
    if (popH > 700.f) popH = 700.f;

    float winW = 1120.f, winH = 800.f;
    popupRect = sf::FloatRect((winW - popW) / 2.f, (winH - popH) / 2.f, popW, popH);

    float rowY = popupRect.top + headerH;
    for (int i = 0; i < (int)options.size(); ++i) {
        optionRects.push_back(sf::FloatRect(popupRect.left + 10.f, rowY, popW - 20.f, rowH));
        rowY += rowH + padding;
    }

    cancelBtnRect = sf::FloatRect(popupRect.left + popW / 2.f - 60.f, rowY + 5.f, 120.f, 34.f);
}

void ListChoicePopup::render(sf::RenderWindow& window) {
    draw3DPanel(window, popupRect, sf::Color(200, 200, 200), false);

    draw3DPanel(window, sf::FloatRect(popupRect.left, popupRect.top, popupRect.width, 50.f), sf::Color(30, 80, 180), false);
    window.draw(createText(promptText, popupRect.left + 10.f, popupRect.top + 10.f, 16, sf::Color::White));

    for (int i = 0; i < (int)options.size(); ++i) {
        sf::Color rowColor = (i % 2 == 0) ? sf::Color(240, 240, 240) : sf::Color(225, 225, 225);
        draw3DPanel(window, optionRects[i], rowColor, false);
        
        std::string fullText = std::to_string(i + 1) + ". " + options[i];
        std::vector<std::string> wrapped = wrapTextToWidth(mainFont, 14, fullText, optionRects[i].width - 15.f);
        if (!wrapped.empty()) {
            window.draw(createText(wrapped[0] + (wrapped.size() > 1 ? "..." : ""), optionRects[i].left + 8.f, optionRects[i].top + 7.f, 14, sf::Color::Black));
        }
    }

    draw3DPanel(window, cancelBtnRect, sf::Color(180, 60, 60), false);
    window.draw(createText("CANCEL (0)", cancelBtnRect.left + 14.f, cancelBtnRect.top + 7.f, 14, sf::Color::White));
}

PopupResult ListChoicePopup::handleMouseClick(float mouseX, float mouseY) {
    if (containsPoint(cancelBtnRect, mouseX, mouseY)) {
        selectedIdx = -1;
        return PopupResult::INPUT_SUBMIT;
    }
    for (int i = 0; i < (int)optionRects.size(); ++i) {
        if (containsPoint(optionRects[i], mouseX, mouseY)) {
            selectedIdx = i;
            return PopupResult::INPUT_SUBMIT;
        }
    }
    return PopupResult::NONE;
}
