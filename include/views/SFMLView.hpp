#pragma once

// Library
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <array>
#include <algorithm>

// SFML, Interface
#include <SFML/Graphics.hpp>
#include "GameView.hpp"

// Komponen GUI, Popups
#include "PropertyTile.hpp"
#include "StreetTile.hpp"

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

class SFMLView : public GameView {
private:
    sf::RenderWindow window;
    
    // Objek GUI
    Board boardGame;
    SidePanel rightSide;
    MenuBar menu;
    Popup* activePopup = nullptr;

    // Status 
	AppState appState;
    bool isMenuBarVisible;

    std::array<int, 4> visualPlayerPositions;
    std::array<bool, 4> visualPlayerActive;
	std::array<StatusProperty, 40> visualPropertyData; 

    int currentTurnVisual = 0;
    int lastRolledPlayer = 0;
    int auctionPassCount = 0;
    int lastPopupResponse = -1;
    int lastPopupTileIndex = -1;
    std::string lastPopupInputData = "";
    GameContext* latestContext = nullptr;

    // Animation
    sf::Clock frameClock; 
    bool isAnimatingToken = false;
    int animatingPlayerIdx = -1;
    float hopProgress = 0.0f; // Dari 0.0 (mulai) ke 1.0 (mendarat)

    std::vector<int> animationPath; 
    sf::Vector2f startVisualPos;
    sf::Vector2f targetVisualPos;

    std::array<PlayerDirection, 4> playerDirections = {
        PlayerDirection::Forward, PlayerDirection::Forward, 
        PlayerDirection::Forward, PlayerDirection::Forward
    };

    // Helper
    void updateTokenAnimation(float deltaTime);
    PlayerDirection getDirectionForTile(int nextTile);

public:
    SFMLView();
    ~SFMLView() override;

    // GUI 
    void renderBoard();
    void processInput();
    bool isWindowOpen() const;

    // Implementasi GameView
    void updateBoardState(GameContext& G) override;
    void showDiceAnimation(int dice1, int dice2) override;
    void triggerPopup(const std::string& popupType, Tile* tileData, const std::string& popupMessage = "") override;
    void closeUI() override;
    bool isAnimationPlaying() const override;
    int getPopupResponse() override;
    int getLastPopupTileIndex() const override;
    std::string getGUICommandString();
    CommandType getGUICommand();
    
    std::string getStringInput(const std::string& prompt) override;
    int getAuctionBid(int minBid, int activePlayerIdx, const std::string& propName, int currentMaxBid, int highBidderIdx) override;
    int getSkillCardChoice(const std::vector<SkillCard*>& cards, bool isDiscardMode) override;
    int getIntChoiceFromList(const std::string& prompt, const std::vector<std::string>& options) override;
    void showInfoPopup(const std::string& title, const std::string& message) override;

    std::string getLastPopupInputData() const { return lastPopupInputData; }
    void clearLastPopupInputData() { lastPopupInputData = ""; }

    void addLog(const std::string& msg) override;
};