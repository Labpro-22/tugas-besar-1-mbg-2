#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <array>

enum class MenuAction {
    NONE,
    NEW_GAME,
    SAVE_GAME,
    LOAD_GAME,
    ABOUT,
    QUIT
};

class MenuBar {
private:
    sf::FloatRect topBarArea;
    sf::FloatRect gameButtonRect;
    std::array<sf::FloatRect, 5> dropdownItemRects;
    std::array<std::string, 5> dropdownLabels;
    
    sf::Font mainFont;
    bool isDropdownOpen;

    // Helper methods
    void draw3DPanel(sf::RenderWindow& window, sf::FloatRect rect, sf::Color baseColor, bool isSunken = false) const;
    sf::Text createText(const std::string& text, float x, float y, unsigned int size, const sf::Color& color) const;
    bool containsPoint(const sf::FloatRect& rect, float x, float y) const;

public:
    MenuBar();

    void loadAssets();
    void fitToLayout(float windowWidth, float menuHeight);
    
    void render(sf::RenderWindow& window);
    
    // Input handling
    void handleMouseMove(float mouseX, float mouseY);
    MenuAction handleMouseClick(float mouseX, float mouseY);

    // getter
    bool isMenuDropdownOpen() const;
};