#include "../../include/views/gui/MenuBar.hpp"
#include <iostream>

MenuBar::MenuBar() {
    isDropdownOpen = false;
    dropdownLabels = {"New Game", "Save Game", "Load Game", "About", "Quit"};
}

void MenuBar::loadAssets() {
    if (!mainFont.loadFromFile("assets/Roboto-Black.ttf")) {
        std::cerr << "ERROR: Failed to load font in MenuBar.\n";
    }
}

void MenuBar::fitToLayout(float windowWidth, float menuHeight) {
    topBarArea = sf::FloatRect(0.f, 0.f, windowWidth, menuHeight);
    
    float buttonWidth = 100.f;
    gameButtonRect = sf::FloatRect(10.f, 0.f, buttonWidth, menuHeight);

    float dropdownY = topBarArea.top + topBarArea.height;
    float itemHeight = 35.f;
    
    for (int i = 0; i < 5; ++i) {
        dropdownItemRects[i] = sf::FloatRect(10.f, dropdownY + (i * itemHeight), 200.f, itemHeight);
    }
}


bool MenuBar::containsPoint(const sf::FloatRect& rect, float x, float y) const {
    return x >= rect.left && x <= rect.left + rect.width && y >= rect.top && y <= rect.top + rect.height;
}

sf::Text MenuBar::createText(const std::string& text, float x, float y, unsigned int size, const sf::Color& color) const {
    sf::Text sfText;
    sfText.setFont(mainFont);
    sfText.setString(text);
    sfText.setPosition(x, y);
    sfText.setCharacterSize(size);
    sfText.setFillColor(color);
    return sfText;
}

void MenuBar::draw3DPanel(sf::RenderWindow& window, sf::FloatRect rect, sf::Color baseColor, bool isSunken) const {
    sf::RectangleShape panel(sf::Vector2f(rect.width, rect.height));
    panel.setPosition(rect.left, rect.top);
    panel.setFillColor(baseColor);
    window.draw(panel);

    sf::Color colorTopLeft = isSunken ? sf::Color(100, 100, 100) : sf::Color::White;
    sf::Color colorBottomRight = isSunken ? sf::Color::White : sf::Color(100, 100, 100);

    sf::RectangleShape highlightTop(sf::Vector2f(rect.width, 2.f));
    highlightTop.setPosition(rect.left, rect.top);
    highlightTop.setFillColor(colorTopLeft);

    sf::RectangleShape highlightLeft(sf::Vector2f(2.f, rect.height));
    highlightLeft.setPosition(rect.left, rect.top);
    highlightLeft.setFillColor(colorTopLeft);

    sf::RectangleShape shadowBottom(sf::Vector2f(rect.width, 2.f));
    shadowBottom.setPosition(rect.left, rect.top + rect.height - 2.f);
    shadowBottom.setFillColor(colorBottomRight);

    sf::RectangleShape shadowRight(sf::Vector2f(2.f, rect.height));
    shadowRight.setPosition(rect.left + rect.width - 2.f, rect.top);
    shadowRight.setFillColor(colorBottomRight);

    window.draw(highlightTop);
    window.draw(highlightLeft);
    window.draw(shadowBottom);
    window.draw(shadowRight);
}

void MenuBar::render(sf::RenderWindow& window) {
    draw3DPanel(window, topBarArea, sf::Color(192, 192, 192), false);

    sf::Color buttonColor = isDropdownOpen ? sf::Color(160, 160, 160) : sf::Color(192, 192, 192);
    draw3DPanel(window, gameButtonRect, buttonColor, isDropdownOpen); // Sunken when open
    window.draw(createText("Game", gameButtonRect.left + 15.f, gameButtonRect.top + 8.f, 18, sf::Color::Black));

    if (isDropdownOpen) {
        sf::FloatRect fullDropdownRect(10.f, topBarArea.height, 200.f, 35.f * 5.f);
        draw3DPanel(window, fullDropdownRect, sf::Color(210, 210, 210), false);

        for (int i = 0; i < 5; ++i) {
            if (i > 0) {
                sf::RectangleShape separator(sf::Vector2f(196.f, 2.f));
                separator.setPosition(12.f, dropdownItemRects[i].top);
                separator.setFillColor(sf::Color(150, 150, 150));
                window.draw(separator);
            }
            
            window.draw(createText(dropdownLabels[i], dropdownItemRects[i].left + 15.f, dropdownItemRects[i].top + 6.f, 16, sf::Color::Black));
        }
    }
}

void MenuBar::handleMouseMove(float mouseX, float mouseY) {
    bool isHoveringButton = containsPoint(gameButtonRect, mouseX, mouseY);
    
    sf::FloatRect fullDropdownRect(10.f, topBarArea.height, 200.f, 35.f * 5.f);
    bool isHoveringDropdown = isDropdownOpen && containsPoint(fullDropdownRect, mouseX, mouseY);

    if (isHoveringButton || isHoveringDropdown) {
        isDropdownOpen = true;
    } else {
        isDropdownOpen = false;
    }
}

MenuAction MenuBar::handleMouseClick(float mouseX, float mouseY) {
    if (isDropdownOpen) {
        for (int i = 0; i < 5; ++i) {
            if (containsPoint(dropdownItemRects[i], mouseX, mouseY)) {
                isDropdownOpen = false; 
                
                if (i == 0) return MenuAction::NEW_GAME;
                if (i == 1) return MenuAction::SAVE_GAME;
                if (i == 2) return MenuAction::LOAD_GAME;
                if (i == 3) return MenuAction::ABOUT;
                if (i == 4) return MenuAction::QUIT;
            }
        }
    }
    
    return MenuAction::NONE;
}

bool MenuBar::isMenuDropdownOpen() const {
    return isDropdownOpen;
}