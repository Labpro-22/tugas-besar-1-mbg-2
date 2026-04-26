#pragma once
#include <SFML/Graphics.hpp>
#include <string>

enum class PopupResult {
    NONE,
    BUY_PROPERTY,
    AUCTION_PROPERTY,
    OK_CLOSE,
    YEAH_CLOSE,
    TAX_FLAT,
    TAX_PERCENTAGE,
    MORTGAGE_PROPERTY,
    SELL_BUILDINGS,
    CLOSE_POPUP,
    AUCTION_BID,
    AUCTION_PASS,
    APPLY_FESTIVAL,
    INPUT_SUBMIT
};

class Popup {
protected:
    sf::FloatRect popupRect;
    sf::Font mainFont;
    bool isVisible;

    void draw3DPanel(sf::RenderWindow& window, sf::FloatRect rect, sf::Color baseColor, bool isSunken = false) const;
    sf::Text createText(const std::string& text, float x, float y, unsigned int size, const sf::Color& color) const;
    bool containsPoint(const sf::FloatRect& rect, float x, float y) const;

public:
    Popup();
    virtual ~Popup() = default;

    virtual void loadAssets();
    
    static std::vector<std::string> wrapTextToWidth(const sf::Font& font, unsigned int size, const std::string& text, float maxWidth);

    virtual void render(sf::RenderWindow& window) = 0;
    virtual PopupResult handleMouseClick(float mouseX, float mouseY) = 0;
    virtual void handleTextInput(unsigned int unicode) {}
    virtual std::string getPopupInputData() { return ""; }

    void renderOverlay(sf::RenderWindow& window, float windowWidth, float windowHeight);
};