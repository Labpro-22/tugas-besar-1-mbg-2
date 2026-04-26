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
    lastPopupTileIndex = -1;
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
                    else if (res == PopupResult::TAX_FLAT) lastPopupResponse = 1;
                    else if (res == PopupResult::TAX_PERCENTAGE) lastPopupResponse = 2;
                    else if (res == PopupResult::AUCTION_BID) lastPopupResponse = 3;
                    else if (res == PopupResult::AUCTION_PASS) lastPopupResponse = 4;
                    else if (res == PopupResult::APPLY_FESTIVAL) lastPopupResponse = 1;
                    else lastPopupResponse = 0;

                    if (FestivalPopup* festivalPopup = dynamic_cast<FestivalPopup*>(activePopup)) {
                        lastPopupTileIndex = festivalPopup->getSelectedTileIndex();
                    }

                    delete activePopup;
                    activePopup = nullptr;
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
    latestContext = &G;
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
    for (auto& status : visualPropertyData) {
        status.setOwnerIndex(-1);
        status.setLevel(0);
    }

    for (int tileIdx = 0; tileIdx < G.getBoard().getTotalTile() && tileIdx < 40; ++tileIdx) {
        Tile* tile = G.getBoard().getTile(tileIdx);
        PropertyTile* property = dynamic_cast<PropertyTile*>(tile);
        if (property == nullptr) continue;

        Player* owner = property->getOwner();
        if (owner == nullptr) continue;

        int ownerIdx = -1;
        for (size_t pIdx = 0; pIdx < players.size() && pIdx < 4; ++pIdx) {
            if (&players[pIdx] == owner) {
                ownerIdx = static_cast<int>(pIdx);
                break;
            }
        }
        if (ownerIdx < 0) continue;

        int level = 1;
        if (StreetTile* street = dynamic_cast<StreetTile*>(property)) {
            if (street->getHasHotel()) {
                level = 5;
            } else {
                level = std::max(1, std::min(4, street->getHouseCount() + 1));
            }
        }

        visualPropertyData[tileIdx].setOwnerIndex(ownerIdx);
        visualPropertyData[tileIdx].setLevel(level);
    }

    for (size_t i = 0; i < players.size() && i < 4; ++i) {
        rightSide.setPlayerBalance(i, players[i].getBalance());

        if (players[i].getStatus() != PlayerStatus::BANKRUPT) {
            visualPlayerActive[i] = true;
        } else {
            rightSide.setPlayerData(i, players[i].getName(), false);
        }

        visualPlayerPositions[i] = players[i].getPosition();
    }
}

void SFMLView::showDiceAnimation(int dice1, int dice2) {
    rightSide.setDiceResult(dice1, dice2);

    int totalRoll = dice1 + dice2;
    if (totalRoll <= 0) {
        return;
    }

    int startTile = visualPlayerPositions[currentTurnVisual];
    int finalTile = (startTile + totalRoll) % 40;
    
    isAnimatingToken = true;
    animatingPlayerIdx = currentTurnVisual;
    int animatedPlayerIdx = animatingPlayerIdx;
    animationPath.clear();
    hopProgress = 0.0f;
    
    for (int i = 1; i <= totalRoll; ++i) {
        animationPath.push_back((startTile + i) % 40);
    }
    
    startVisualPos = boardGame.getTokenPosition(startTile) + boardGame.computeTokenOffsetForStackIndex(animatingPlayerIdx);
    int nextTile = animationPath.front();
    targetVisualPos = boardGame.getTokenPosition(nextTile) + boardGame.computeTokenOffsetForStackIndex(animatingPlayerIdx);

    sf::Clock animClock;
    sf::Clock waitClock;
    
    while (window.isOpen() && (isAnimatingToken || waitClock.getElapsedTime().asSeconds() < 1.0f)) {
        float dt = animClock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return;
            }
        }

        if (isAnimatingToken) {
            updateTokenAnimation(dt); 
        }
        
        rightSide.update(); 
        renderBoard();
    }

    if (animatedPlayerIdx >= 0 && animatedPlayerIdx < static_cast<int>(playerDirections.size())) {
        playerDirections[animatedPlayerIdx] = getDirectionForTile(finalTile);
    }
}

void SFMLView::triggerPopup(const std::string& popupType, Tile* tileData, const std::string& popupMessage) {
    if (activePopup != nullptr) {
        delete activePopup;
        activePopup = nullptr;
    }

    lastPopupResponse = -1;
    lastPopupTileIndex = -1;

    const std::string activePlayerName = rightSide.getPlayerName(currentTurnVisual);

    if (popupType == "PROPERTY") {
        PropertyTile* pTile = dynamic_cast<PropertyTile*>(tileData);

        if (pTile != nullptr) {
            std::string namaTanah = pTile->getName();
            int hargaTanah = pTile->getPrice();

            activePopup = new PropertyPopup(namaTanah, hargaTanah, activePlayerName);
        }
    } else if (popupType == "LANDING") {
        std::string tileInfo = "Unknown Tile";
        if (tileData != nullptr) {
            tileInfo = tileData->getName() + " (" + tileData->getCode() + ")";
        }
        std::string message = popupMessage.empty() ? ("You landed on " + tileInfo) : popupMessage;
        activePopup = new AlertPopup("Tile", message, activePlayerName, "OK");
    } else if (popupType == "TAX_PPH") {
        activePopup = new AlertPopup("Tax", "Choose payment mode", activePlayerName, "% Wealth", "Flat");
    } else if (popupType == "TAX_PPBM") {
        activePopup = new AlertPopup("Tax", "You must pay PBM luxury tax.", activePlayerName, "OK");
    } else if (popupType == "JAIL") {
        activePopup = new AlertPopup("Jail", "You are sent to jail.", activePlayerName, "OK");
    } else if (popupType == "CHANCE") {
        std::string desc = popupMessage.empty() ? "Take and apply the chance card." : popupMessage;
        activePopup = new CardPopup("Kesempatan", desc, "OK");
    } else if (popupType == "COMMUNITY") {
        std::string desc = popupMessage.empty() ? "Take and apply the community card." : popupMessage;
        activePopup = new CardPopup("Dana Umum", desc, "OK");
    } else if (popupType == "AUCTION") {
        PropertyTile* pTile = dynamic_cast<PropertyTile*>(tileData);
        std::string propertyName = (pTile != nullptr) ? pTile->getName() : "Property";
        int starterIdx = 0;
        if (latestContext != nullptr && !latestContext->getPlayers().empty()) {
            starterIdx = (latestContext->getCurrentPlayerIndex() + 1) % latestContext->getPlayers().size();
        }
        activePopup = new AuctionPopup(propertyName, 0, starterIdx);
    } else if (popupType == "INFO") {
        std::string message = popupMessage.empty() ? "Feature is not available in GUI yet." : popupMessage;
        activePopup = new AlertPopup("Info", message, activePlayerName, "OK");
    } else if (popupType == "FESTIVAL") {
        std::vector<FestivalItem> items;

        if (latestContext != nullptr) {
            Player& current = latestContext->getCurrentPlayer();
            for (PropertyTile* prop : current.getOwnedProperties()) {
                StreetTile* street = dynamic_cast<StreetTile*>(prop);
                if (street == nullptr) continue;

                int baseRent = 0;
                const std::vector<int> rentPrices = street->getRentPrices();
                if (street->getHasHotel() && rentPrices.size() > 5) {
                    baseRent = rentPrices[5];
                } else {
                    int houseCount = street->getHouseCount();
                    if (houseCount >= 0 && houseCount < static_cast<int>(rentPrices.size())) {
                        baseRent = rentPrices[houseCount];
                    }
                }

                FestivalItem item;
                item.tileIndex = street->getIdx();
                item.name = street->getName() + " (" + street->getCode() + ")";
                item.baseRent = baseRent;
                item.currentMultiplier = std::max(1, street->getFestivalStack());
                item.currentDuration = street->getFestivalTurn();
                items.push_back(item);
            }
        }

        if (items.empty()) {
            activePopup = new AlertPopup("Festival", "No eligible street property", activePlayerName, "OK");
        } else {
            activePopup = new FestivalPopup(items);
        }
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
    while (window.isOpen() && activePopup != nullptr) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) { 
                window.close(); 
                return 0; 
            }
            
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                PopupResult res = activePopup->handleMouseClick(mPos.x, mPos.y);
                
                if (res != PopupResult::NONE) {
                    if (res == PopupResult::BUY_PROPERTY) {
                        lastPopupResponse = 1;
                    } else if (res == PopupResult::AUCTION_PROPERTY) {
                        lastPopupResponse = 2;
                    } else if (res == PopupResult::TAX_FLAT) {
                        lastPopupResponse = 1;
                    } else if (res == PopupResult::TAX_PERCENTAGE) {
                        lastPopupResponse = 2;
                    } else if (res == PopupResult::AUCTION_BID) {
                        lastPopupResponse = 3;
                    } else if (res == PopupResult::AUCTION_PASS) {
                        lastPopupResponse = 4;
                    } else if (res == PopupResult::APPLY_FESTIVAL) {
                        lastPopupResponse = 1;
                    } else {
                        lastPopupResponse = 0;
                    }

                    if (FestivalPopup* festivalPopup = dynamic_cast<FestivalPopup*>(activePopup)) {
                        lastPopupTileIndex = festivalPopup->getSelectedTileIndex();
                    }
                    
                    delete activePopup; 
                    activePopup = nullptr;
                    return lastPopupResponse;
                }
            }
        }
        renderBoard(); 
    }
    return lastPopupResponse;
}

int SFMLView::getLastPopupTileIndex() const {
    return lastPopupTileIndex;
}

CommandType SFMLView::getGUICommand() {
    while (window.isOpen()) {
        float deltaTime = frameClock.restart().asSeconds();
        updateTokenAnimation(deltaTime); 
        rightSide.update();

        if (activePopup != nullptr) {
            sf::Event popupEvent;
            while (window.pollEvent(popupEvent)) {
                if (popupEvent.type == sf::Event::Closed) {
                    window.close();
                    return CommandType::UNKNOWN_COMMAND;
                }

                if (popupEvent.type == sf::Event::MouseButtonPressed && popupEvent.mouseButton.button == sf::Mouse::Left) {
                    PopupResult res = activePopup->handleMouseClick(popupEvent.mouseButton.x, popupEvent.mouseButton.y);
                    if (res != PopupResult::NONE) {
                        if (res == PopupResult::BUY_PROPERTY) lastPopupResponse = 1;
                        else if (res == PopupResult::AUCTION_PROPERTY) lastPopupResponse = 2;
                        else if (res == PopupResult::TAX_FLAT) lastPopupResponse = 1;
                        else if (res == PopupResult::TAX_PERCENTAGE) lastPopupResponse = 2;
                        else if (res == PopupResult::AUCTION_BID) lastPopupResponse = 3;
                        else if (res == PopupResult::AUCTION_PASS) lastPopupResponse = 4;
                        else if (res == PopupResult::APPLY_FESTIVAL) lastPopupResponse = 1;
                        else lastPopupResponse = 0;

                        if (FestivalPopup* festivalPopup = dynamic_cast<FestivalPopup*>(activePopup)) {
                            lastPopupTileIndex = festivalPopup->getSelectedTileIndex();
                        }

                        delete activePopup;
                        activePopup = nullptr;
                    }
                }
            }

            renderBoard();
            continue;
        }

        std::string cmdStr = rightSide.pollCommandString();
        if (!cmdStr.empty()) {
            std::string rawInput = cmdStr;
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
            if (cmdStr == "HELP") return CommandType::HELP;

            if (cmdStr.rfind("ATUR_DADU", 0) == 0) {
                triggerPopup("INFO", nullptr, "ATUR_DADU in GUI is not supported yet. Use LEMPAR_DADU.");
                rightSide.addHistoryEntry("[INFO] ATUR_DADU belum didukung di GUI.");
                continue;
            }
            if (cmdStr.rfind("SIMPAN", 0) == 0) {
                triggerPopup("INFO", nullptr, "SIMPAN via GUI input is not supported yet.");
                rightSide.addHistoryEntry("[INFO] SIMPAN via GUI belum didukung.");
                continue;
            }
            if (cmdStr.rfind("CETAK_LOG", 0) == 0) {
                triggerPopup("INFO", nullptr, "CETAK_LOG via GUI input is not supported yet.");
                rightSide.addHistoryEntry("[INFO] CETAK_LOG via GUI belum didukung.");
                continue;
            }
            if (cmdStr.rfind("BID", 0) == 0 || cmdStr == "PASS") {
                triggerPopup("INFO", nullptr, "BID/PASS command in panel is disabled. Use auction popup buttons.");
                rightSide.addHistoryEntry("[INFO] Gunakan tombol pada popup auction untuk BID/PASS.");
                continue;
            }
            
            rightSide.addHistoryEntry("[ERROR] Invalid Command: " + rawInput);
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

            // // Shortcut Hotkeys
            // if (event.type == sf::Event::KeyPressed) {
            //     switch (event.key.code) {
            //         case sf::Keyboard::Space: return CommandType::LEMPAR_DADU;
            //         case sf::Keyboard::E:     return CommandType::AKHIRI_GILIRAN;
            //         case sf::Keyboard::B:     return CommandType::BANGUN;
            //         case sf::Keyboard::P:     return CommandType::CETAK_PROPERTI;
            //         case sf::Keyboard::A:     return CommandType::CETAK_AKTA;
            //         case sf::Keyboard::K:     return CommandType::GUNAKAN_KEMAMPUAN;
            //         case sf::Keyboard::S:     return CommandType::SIMPAN;
            //         default: break;
            //     }
            // }
        }

        renderBoard();
    }

    return CommandType::UNKNOWN_COMMAND;
}

void SFMLView::addLog(const std::string& msg) {
    rightSide.addHistoryEntry(msg);
}