#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>

#include "gui/Board.hpp"
#include "gui/SidePanel.hpp"
#include "gui/MenuBar.hpp"
#include "gui/popups/Popup.hpp"
#include "gui/popups/PropertyPopup.hpp"
#include "gui/popups/CardPopup.hpp"
#include "gui/popups/AlertPopup.hpp"
#include "gui/popups/AuctionPopup.hpp"
#include "gui/popups/MortgagePopup.hpp"
#include "gui/popups/FestivalPopup.hpp"
#include "gui/popups/akta/PropertyAktaPopup.hpp"

enum class AppState { IDLE, SETUP, PLAYING };
class SFMLView {
private:
    sf::RenderWindow window;
    
    // Objek komponen GUI
    Board boardGame;
    SidePanel rightSide;
    MenuBar menu;
    bool isMenuBarVisible;

	AppState appState;
    std::array<int, 4> mockPlayerPositions;
    std::array<bool, 4> mockPlayerActive;
    int currentTurn;

	Popup* activePopup = nullptr;
	std::array<PropertyStatus, 40> mockPropertyData; 
    int lastRolledPlayer = 0;

	void triggerPopupForTile(int tileIndex, int playerIdx);
	int auctionPassCount = 0;

    sf::Clock frameClock; 
    
    // Status Animasi
    bool isAnimatingToken = false;
    int animatingPlayerIdx = -1;
    std::vector<int> animationPath; // Daftar petak yang akan dilompati
    
    // Matematika Lompatan
    float hopProgress = 0.0f; // Dari 0.0 (mulai) ke 1.0 (mendarat)
    sf::Vector2f startVisualPos;
    sf::Vector2f targetVisualPos;
    
    std::array<PlayerDirection, 4> playerDirections = {
        PlayerDirection::Forward, PlayerDirection::Forward, 
        PlayerDirection::Forward, PlayerDirection::Forward
    };

    void updateTokenAnimation(float deltaTime);
    PlayerDirection getDirectionForTile(int nextTile);
    
public:
    SFMLView();
    ~SFMLView();

    void renderBoard();
    void processInput();

    bool isWindowOpen() const;
};