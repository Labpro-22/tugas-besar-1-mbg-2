#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <array>

enum class PanelState { PLAYER_SETUP, IN_GAME };

class SidePanel {
private:
    sf::FloatRect panelArea;
    sf::Font mainFont;
    PanelState currentState;
    
    // --- UI Areas ---
    std::array<sf::FloatRect, 4> avatarSelectRects; // Grid untuk pilih karakter
    sf::FloatRect takeMoneyButtonRect;              // Tombol Take Money
    sf::FloatRect giveDiceButtonRect;
    sf::FloatRect startGameButtonRect;
    sf::FloatRect rollDiceButtonRect;

    // --- Assets ---
    std::array<sf::Texture, 4> playerTextures;
    std::array<bool, 4> isPlayerTextureLoaded;
    std::array<sf::Texture, 6> diceTextures;
    std::array<bool, 6> isDiceTextureLoaded;

    // --- Game Data ---
    int selectedSetupCharacter;
    int activePlayerIndex; 
    int firstTurnPlayerIndex;
    std::array<std::string, 4> playerNames;
    std::array<bool, 4> isPlayerEnabled;
    std::array<bool, 4> isNameEdited;
    std::array<int, 2> diceValues;
    std::vector<std::string> gameHistory;
    std::string cardInfoText;
    
    // --- Animation & Signals ---
    bool isDiceRolling;
    sf::Clock diceRollClock;
    sf::Clock diceTickClock;
    bool flagStartClicked;
    bool flagRollFinished; 

    void renderSetupState(sf::RenderWindow& window);
    void renderInGameState(sf::RenderWindow& window);
    void draw3DPanel(sf::RenderWindow& window, sf::FloatRect rect, sf::Color baseColor, bool isSunken = false) const;
    sf::Text createText(const std::string& text, float x, float y, unsigned int size, const sf::Color& color) const;
    bool containsPoint(const sf::FloatRect& rect, float x, float y) const;

public:
    SidePanel();

    void loadAssets();
    void fitToLayout(sf::FloatRect containerArea);
    
    void update(); 
    void render(sf::RenderWindow& window);
    void handleMouseClick(float mouseX, float mouseY);
    void handleTextInput(unsigned int unicode);

    void addHistoryEntry(const std::string& entry);
    void setPanelState(PanelState state);
    void resetSetup();
    void setActivePlayerTurn(int index); // Memberitahu UI giliran siapa

    // Getter & Signals
    bool pollStartGameClicked();
    bool pollRollDiceFinished(int& outDice1, int& outDice2); // Sinyal dadu fix
    std::array<bool, 4> getActivePlayers() const; // Siapa aja yang ikut main
    int getFirstTurnPlayerIndex() const;

    std::string getPlayerName(int index) const;
};