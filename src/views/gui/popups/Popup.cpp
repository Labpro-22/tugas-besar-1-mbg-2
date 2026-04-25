#include "gui/popups/Popup.hpp"
#include <iostream>

Popup::Popup() {
    popupRect = sf::FloatRect(310.f, 200.f, 500.f, 350.f);
    isVisible = true;
}

void Popup::loadAssets() {
    if (!mainFont.loadFromFile("assets/Roboto-Black.ttf")) {
        std::cerr << "ERROR: Gagal memuat font di Popup\n";
    }
}

void Popup::renderOverlay(sf::RenderWindow& window, float windowWidth, float windowHeight) {
    sf::RectangleShape overlay(sf::Vector2f(windowWidth, windowHeight));
    overlay.setFillColor(sf::Color(0, 0, 0, 150)); 
    window.draw(overlay);
}

void Popup::draw3DPanel(sf::RenderWindow& window, sf::FloatRect rect, sf::Color baseColor, bool isSunken) const {
    sf::RectangleShape panel(sf::Vector2f(rect.width, rect.height));
    panel.setPosition(rect.left, rect.top);
    panel.setFillColor(baseColor);
    window.draw(panel);

    sf::Color light = isSunken ? sf::Color(100, 100, 100) : sf::Color::White;
    sf::Color dark = isSunken ? sf::Color::White : sf::Color(100, 100, 100);

    sf::RectangleShape lineTop(sf::Vector2f(rect.width, 2.f));
    lineTop.setPosition(rect.left, rect.top);
    lineTop.setFillColor(light);
    
    sf::RectangleShape lineLeft(sf::Vector2f(2.f, rect.height));
    lineLeft.setPosition(rect.left, rect.top);
    lineLeft.setFillColor(light);

    sf::RectangleShape lineBottom(sf::Vector2f(rect.width, 2.f));
    lineBottom.setPosition(rect.left, rect.top + rect.height - 2.f);
    lineBottom.setFillColor(dark);

    sf::RectangleShape lineRight(sf::Vector2f(2.f, rect.height));
    lineRight.setPosition(rect.left + rect.width - 2.f, rect.top);
    lineRight.setFillColor(dark);

    window.draw(lineTop); window.draw(lineLeft);
    window.draw(lineBottom); window.draw(lineRight);
}

sf::Text Popup::createText(const std::string& text, float x, float y, unsigned int size, const sf::Color& color) const {
    sf::Text sfText;
    sfText.setFont(mainFont);
    sfText.setString(text);
    sfText.setPosition(x, y);
    sfText.setCharacterSize(size);
    sfText.setFillColor(color);
    return sfText;
}

bool Popup::containsPoint(const sf::FloatRect& rect, float x, float y) const {
    return x >= rect.left && x <= rect.left + rect.width && y >= rect.top && y <= rect.top + rect.height;
}