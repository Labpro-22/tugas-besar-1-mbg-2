#include "SFMLView.hpp"

// --- Inisialisasi dan Destruktor --- 
SFMLView::SFMLView() {
    window.create(sf::VideoMode(1120, 800), "Nimonsy", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);
    isMenuBarVisible = false;

    // Load aset GUI
    boardGame.loadAssets();
    rightSide.loadAssets();
    menu.loadAssets();

    boardGame.fitToLayout(sf::FloatRect(0.f, 0.f, 800.f, 800.f));
    rightSide.fitToLayout(sf::FloatRect(800.f, 0.f, 320.f, 800.f));
    menu.fitToLayout(1120.f, 40.f);
    
    std::cout << "[GUI] Berhasil inisialisasi SFMLView.\n";

    appState = AppState::IDLE;
    
    // Inisialisasi state visual awal (kosong/default)
    visualPlayerPositions = {0, 0, 0, 0};
    visualPlayerActive = {false, false, false, false};
    currentTurnVisual = 0;

    lastPopupResponse = -1;
}

SFMLView::~SFMLView() {
    if (window.isOpen()) window.close();
    if (activePopup != nullptr) delete activePopup;
}

// --- Render ---
void SFMLView::renderBoard() {
    window.clear(sf::Color(229, 232, 236));

    sf::Vector2f animPos = startVisualPos;
    int currentAnimPlayer = -1; 

    if (isAnimatingToken) {
        currentAnimPlayer = animatingPlayerIdx;
        animPos.x = startVisualPos.x + (targetVisualPos.x - startVisualPos.x) * hopProgress;
        animPos.y = startVisualPos.y + (targetVisualPos.y - startVisualPos.y) * hopProgress;
        
        float hopHeight = 40.0f; 
        animPos.y -= std::sin(hopProgress * 3.14159f) * hopHeight;
    }

    boardGame.render(window, visualPlayerPositions, visualPlayerActive, visualPropertyData, currentAnimPlayer, animPos, playerDirections);

    if (appState != AppState::IDLE) rightSide.render(window);

    if (activePopup != nullptr) {
        activePopup->renderOverlay(window, 1120.f, 800.f);
        activePopup->render(window);
    }

    if (isMenuBarVisible || menu.isMenuDropdownOpen()) menu.render(window);

    window.display();
}

// --- Update Animation ---
PlayerDirection SFMLView::getDirectionForTile(int nextTile) {
    if (nextTile > 0 && nextTile <= 10) return PlayerDirection::Left;    
    if (nextTile > 10 && nextTile <= 20) return PlayerDirection::Back;   
    if (nextTile > 20 && nextTile <= 30) return PlayerDirection::Right;  
    if (nextTile > 30 && nextTile < 40) return PlayerDirection::Forward; 
    return PlayerDirection::Forward; 
}

void SFMLView::updateTokenAnimation(float deltaTime) {
    if (!isAnimatingToken || animationPath.empty()) return;

    float hopSpeed = 3.0f; 
    hopProgress += (deltaTime * hopSpeed);

    if (hopProgress >= 1.0f) {
        hopProgress = 0.0f;
        int reachedTile = animationPath.front();
        startVisualPos = boardGame.getTokenPosition(reachedTile) + boardGame.computeTokenOffsetForStackIndex(animatingPlayerIdx);
        animationPath.erase(animationPath.begin()); 
        
        if (animationPath.empty()) {
            isAnimatingToken = false;
            animatingPlayerIdx = -1; 
        } else {
            int nextTile = animationPath.front();
            targetVisualPos = boardGame.getTokenPosition(nextTile) + boardGame.computeTokenOffsetForStackIndex(animatingPlayerIdx);
            playerDirections[animatingPlayerIdx] = getDirectionForTile(nextTile);
        }
    }
}

// --- Input Handler --- 
void SFMLView::processInput() {
    float deltaTime = frameClock.restart().asSeconds();
    updateTokenAnimation(deltaTime); 
    rightSide.update();

    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) window.close();

        if (activePopup != nullptr) {
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                PopupResult res = activePopup->handleMouseClick(event.mouseButton.x, event.mouseButton.y);
                
                if (res != PopupResult::NONE) {
                    if (res == PopupResult::BUY_PROPERTY) lastPopupResponse = 1;
                    else if (res == PopupResult::AUCTION_PROPERTY) lastPopupResponse = 2;
                    else if (res == PopupResult::OK_CLOSE) lastPopupResponse = 0;
                    else lastPopupResponse = 99; 
                    
                    if (res != PopupResult::AUCTION_BID && res != PopupResult::AUCTION_PASS) {
                        delete activePopup; 
                        activePopup = nullptr; 
                    }
                }
            }
            continue; 
        }

        if (event.type == sf::Event::MouseMoved) {
            float mouseX = event.mouseMove.x;
            float mouseY = event.mouseMove.y;
            isMenuBarVisible = (mouseY <= 40.f || menu.isMenuDropdownOpen());
            if (isMenuBarVisible) menu.handleMouseMove(mouseX, mouseY);
        }

        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            float mouseX = event.mouseButton.x;
            float mouseY = event.mouseButton.y;

            if (isMenuBarVisible || menu.isMenuDropdownOpen()) {
                MenuAction action = menu.handleMouseClick(mouseX, mouseY);
                if (action == MenuAction::QUIT) window.close();
            }

            if (appState != AppState::IDLE && mouseX >= 800.f) {
                rightSide.handleMouseClick(mouseX, mouseY);
            }
        }
    }
}

bool SFMLView::isWindowOpen() const {
    return window.isOpen();
}

// --- GameView ---
void SFMLView::updateBoardState(GameContext& G) {
    std::vector<Player>& players = G.getPlayers();

    if (appState == AppState::IDLE || appState == AppState::SETUP) {
        appState = AppState::PLAYING; 
        rightSide.setPanelState(PanelState::IN_GAME);
        
        for (size_t i = 0; i < 4; ++i) {
            if (i < players.size()) {
                rightSide.setPlayerData(i, players[i].getName(), true);
            } else {
                rightSide.setPlayerData(i, "None", false);
            }
        }
    }

    currentTurnVisual = G.getCurrentPlayerIndex();
    rightSide.setActivePlayerTurn(currentTurnVisual);

    visualPlayerActive.fill(false); // Kalau ada yang bankrut 

    for (size_t i = 0; i < players.size() && i < 4; ++i) {
        visualPlayerPositions[i] = players[i].getPosition();

        if (players[i].getStatus() != PlayerStatus::BANKRUPT){
            visualPlayerActive[i] = true;
        } else {
            rightSide.setPlayerData(i, players[i].getName(), false);
        }
    }
}

void SFMLView::showDiceAnimation(int dice1, int dice2) {
    rightSide.setDiceResult(dice1, dice2);

    int totalRoll = dice1 + dice2;
    int startTile = visualPlayerPositions[currentTurnVisual];
    
    isAnimatingToken = true;
    animatingPlayerIdx = currentTurnVisual;
    animationPath.clear();
    hopProgress = 0.0f;
    
    for (int i = 1; i <= totalRoll; ++i) {
        animationPath.push_back((startTile + i) % 40);
    }
    
    startVisualPos = boardGame.getTokenPosition(startTile) + boardGame.computeTokenOffsetForStackIndex(animatingPlayerIdx);
    int nextTile = animationPath.front();
    targetVisualPos = boardGame.getTokenPosition(nextTile) + boardGame.computeTokenOffsetForStackIndex(animatingPlayerIdx);
    playerDirections[animatingPlayerIdx] = getDirectionForTile(nextTile);
}

void SFMLView::triggerPopup(const std::string& popupType, Tile* tileData) {
    if (activePopup != nullptr) {
        delete activePopup;
        activePopup = nullptr;
    }

    if (activePopup != nullptr) {
        activePopup->loadAssets();
    }
}

void SFMLView::closeUI() {
    window.close();
}

bool SFMLView::isAnimationPlaying() const {
    return isAnimatingToken;
}

int SFMLView::getPopupResponse() {
    int temp = lastPopupResponse;
    lastPopupResponse = -1;
    return temp;
}

CommandType SFMLView::getGUICommand() {
    while (window.isOpen()) {
        float deltaTime = frameClock.restart().asSeconds();
        updateTokenAnimation(deltaTime); 
        rightSide.update();

        std::string cmdStr = rightSide.pollCommandString();
        if (!cmdStr.empty()) {
            std::transform(cmdStr.begin(), cmdStr.end(), cmdStr.begin(), ::toupper);
            
            if (cmdStr == "LEMPAR_DADU") return CommandType::LEMPAR_DADU;
            if (cmdStr == "AKHIRI_GILIRAN") return CommandType::AKHIRI_GILIRAN;
            if (cmdStr == "CETAK_PAPAN") return CommandType::CETAK_PAPAN;
            if (cmdStr == "CETAK_PROPERTI") return CommandType::CETAK_PROPERTI;
            if (cmdStr == "CETAK_AKTA") return CommandType::CETAK_AKTA;
            if (cmdStr == "BANGUN") return CommandType::BANGUN;
            if (cmdStr == "GADAI") return CommandType::GADAI;
            if (cmdStr == "TEBUS") return CommandType::TEBUS;
            if (cmdStr == "GUNAKAN_KEMAMPUAN") return CommandType::GUNAKAN_KEMAMPUAN;
            
            rightSide.addHistoryEntry("[ERROR] Invalid Command.");
        }

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return CommandType::UNKNOWN_COMMAND;
            }

            if (event.type == sf::Event::TextEntered) {
                rightSide.handleTextInput(event.text.unicode);
            }

            if (activePopup != nullptr) {
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    PopupResult res = activePopup->handleMouseClick(event.mouseButton.x, event.mouseButton.y);
                    if (res != PopupResult::NONE) {
                        if (res == PopupResult::BUY_PROPERTY) lastPopupResponse = 1;
                        else if (res == PopupResult::AUCTION_PROPERTY) lastPopupResponse = 2;
                        else lastPopupResponse = 0;

                        if (res != PopupResult::AUCTION_BID && res != PopupResult::AUCTION_PASS) {
                            delete activePopup; activePopup = nullptr; 
                        }
                    }
                }
                continue; 
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                float mouseX = event.mouseButton.x;
                float mouseY = event.mouseButton.y;

                if (mouseX >= 800.f) { 
                    rightSide.handleMouseClick(mouseX, mouseY);
                    
                    if (rightSide.pollRollButtonClicked()) {
                        return CommandType::LEMPAR_DADU;
                    }
                }
            }

            // Shortcut Hotkeys
            if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code) {
                    case sf::Keyboard::Space: return CommandType::LEMPAR_DADU;
                    case sf::Keyboard::E:     return CommandType::AKHIRI_GILIRAN;
                    case sf::Keyboard::B:     return CommandType::BANGUN;
                    case sf::Keyboard::P:     return CommandType::CETAK_PROPERTI;
                    case sf::Keyboard::A:     return CommandType::CETAK_AKTA;
                    case sf::Keyboard::K:     return CommandType::GUNAKAN_KEMAMPUAN;
                    case sf::Keyboard::S:     return CommandType::SIMPAN;
                    default: break;
                }
            }
        }

        renderBoard();
    }

    return CommandType::UNKNOWN_COMMAND;
}