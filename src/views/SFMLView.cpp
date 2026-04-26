// #include "SFMLView.hpp"
// #include <iostream>

// SFMLView::SFMLView() {
//     window.create(sf::VideoMode(1120, 800), "Nimonsy", sf::Style::Titlebar | sf::Style::Close);
//     window.setFramerateLimit(60);
//     isMenuBarVisible = false;

//     // Load aset untuk setiap komponen
//     boardGame.loadAssets();
//     rightSide.loadAssets();
//     menu.loadAssets();

//     boardGame.fitToLayout(sf::FloatRect(0.f, 0.f, 800.f, 800.f));
//     rightSide.fitToLayout(sf::FloatRect(800.f, 0.f, 320.f, 800.f));
//     menu.fitToLayout(1120.f, 40.f);
    
//     std::cout << "[GUI] Berhasil inisialisasi standalone mockup.\n";

//     appState = AppState::IDLE;
//     mockPlayerPositions = {0, 0, 0, 0};
//     mockPlayerActive = {true, true, true, true};
//     currentTurn = 0;

//     mockPropertyData[38].ownerIndex = 0; 
//     mockPropertyData[38].level = 5; 

//     mockPropertyData[32].ownerIndex = 1;
//     mockPropertyData[32].level = 3;

//     mockPropertyData[7].ownerIndex = 2;
//     mockPropertyData[7].level = 1;
    
//     mockPropertyData[4].ownerIndex = 3;
//     mockPropertyData[4].level = 4;
// }

// SFMLView::~SFMLView() {
//     if (window.isOpen()) window.close();
// }

// void SFMLView::renderBoard() {
//     window.clear(sf::Color(229, 232, 236));

//     sf::Vector2f animPos = startVisualPos;
//     int currentAnimPlayer = -1; 

//     if (isAnimatingToken) {
//         currentAnimPlayer = animatingPlayerIdx;
//         animPos.x = startVisualPos.x + (targetVisualPos.x - startVisualPos.x) * hopProgress;
//         animPos.y = startVisualPos.y + (targetVisualPos.y - startVisualPos.y) * hopProgress;
        
//         float hopHeight = 40.0f; 
//         animPos.y -= std::sin(hopProgress * 3.14159f) * hopHeight;
//     }

//     boardGame.render(window, mockPlayerPositions, mockPlayerActive, mockPropertyData, currentAnimPlayer, animPos, playerDirections);

//     if (appState != AppState::IDLE) {
//         rightSide.render(window);
//     }

//     if (activePopup != nullptr) {
//         activePopup->renderOverlay(window, 1120.f, 800.f);
//         activePopup->render(window);
//     }

//     if (isMenuBarVisible || menu.isMenuDropdownOpen()) {
//         menu.render(window);
//     }

//     window.display();
// }

// void getMockTileData(int index, std::string& outName, int& outPrice, bool& outIsProperty) {
//     outIsProperty = false;
//     outPrice = 0;
    
//     switch (index) {
//         case 2:  outName = "Garut (GRT)"; outPrice = 60; outIsProperty = true; break;
//         case 4:  outName = "Tasikmalaya (TSK)"; outPrice = 60; outIsProperty = true; break;
//         case 7:  outName = "Bogor (BGR)"; outPrice = 100; outIsProperty = true; break;
//         case 32: outName = "Bandung (BDG)"; outPrice = 300; outIsProperty = true; break;
//         case 38: outName = "Jakarta (JKT)"; outPrice = 400; outIsProperty = true; break;
//         case 40: outName = "Ibu Kota Nusantara (IKN)"; outPrice = 400; outIsProperty = true; break;
//         case 3:  outName = "Dana Umum"; break; 
//         case 8:  outName = "Festival"; break;
//         default: outName = "Petak " + std::to_string(index); outPrice = 150; outIsProperty = true; break; 
//     }
// }

// void SFMLView::triggerPopupForTile(int tileIndex, int playerIdx) {
//     std::string playerName = rightSide.getPlayerName(playerIdx);
//     std::string tileName;
//     int tilePrice;
//     bool isProperty;
    
//     getMockTileData(tileIndex, tileName, tilePrice, isProperty);

//     if (isProperty) {
//         int currentOwner = mockPropertyData[tileIndex].ownerIndex;
//         if (currentOwner == -1) {
//             activePopup = new PropertyPopup(tileName, tilePrice, playerName);
//         } else if (currentOwner == playerIdx) {
//             rightSide.addHistoryEntry(playerName + " mendarat di properti miliknya.");
//             return; 
//         } else {
//             rightSide.addHistoryEntry(playerName + " bayar sewa ke " + rightSide.getPlayerName(currentOwner));
//             return;
//         }
//     } else {

//         if (tileName == "Festival") {
//             std::vector<FestivalItem> mockFestProps;
            
//             mockFestProps.push_back({38, "Jakarta (JKT)", 5000, 1, 0});
            
//             mockFestProps.push_back({32, "Bandung (BDG)", 5000, 2, 1});
            
//             mockFestProps.push_back({33, "Denpasar (DEN)", 5000, 8, 2});

//             activePopup = new FestivalPopup(mockFestProps);
//         }
//         else if (tileName == "Dana Umum") {
//             activePopup = new CardPopup("Dana Umum", "LIFE INSURANCE MATURES\nCOLLECT $100", "Yeah!");
//         } 
//         else if (tileName == "Kesempatan") {
//             activePopup = new CardPopup("Kesempatan", "GO DIRECTLY TO JAIL!\nDO NOT PASS GO.", "Darn!");
//         }
//         else if (tileName == "Pajak Penghasilan") {
//             activePopup = new AlertPopup("Income Tax", "Bayar pajak flat $200 atau\n10% dari total kekayaan.", playerName, "10%", "$200");
//         }
//         else if (tileName == "Pajak Barang Mewah") {
//             activePopup = new AlertPopup("Luxury Tax", "Pajak Barang Mewah!\nBayar sebesar $150.", playerName, "Ok");
//         }
//     }

//     if (activePopup) activePopup->loadAssets();
// }

// PlayerDirection SFMLView::getDirectionForTile(int nextTile) {
//     if (nextTile > 0 && nextTile <= 10) return PlayerDirection::Left;    
//     if (nextTile > 10 && nextTile <= 20) return PlayerDirection::Back;   
//     if (nextTile > 20 && nextTile <= 30) return PlayerDirection::Right;  
//     if (nextTile > 30 && nextTile < 40) return PlayerDirection::Forward; 
//     return PlayerDirection::Forward; 
// }

// void SFMLView::updateTokenAnimation(float deltaTime) {
//     if (!isAnimatingToken || animationPath.empty()) return;

//     float hopSpeed = 3.0f; 
//     hopProgress += (deltaTime * hopSpeed);

//     if (hopProgress >= 1.0f) {
//         hopProgress = 0.0f;
//         int reachedTile = animationPath.front();
        
//         startVisualPos = boardGame.getTokenPosition(reachedTile) + boardGame.computeTokenOffsetForStackIndex(animatingPlayerIdx);
        
//         animationPath.erase(animationPath.begin()); 
        
//         if (animationPath.empty()) {
//             isAnimatingToken = false;
//             int finalTile = mockPlayerPositions[animatingPlayerIdx];
//             triggerPopupForTile(finalTile, animatingPlayerIdx);
            
//             animatingPlayerIdx = -1; 
//         } else {
//             int nextTile = animationPath.front();
//             targetVisualPos = boardGame.getTokenPosition(nextTile) + boardGame.computeTokenOffsetForStackIndex(animatingPlayerIdx);
//             playerDirections[animatingPlayerIdx] = getDirectionForTile(nextTile);
//         }
//     }
// }

// void SFMLView::processInput() {
//     float deltaTime = frameClock.restart().asSeconds();
    
//     updateTokenAnimation(deltaTime); 

//     rightSide.update();

//     int dice1, dice2;
//     if (rightSide.pollRollDiceFinished(dice1, dice2) && !isAnimatingToken) {
//         int totalRoll = dice1 + dice2;
//         lastRolledPlayer = currentTurn; 
        
//         int startTile = mockPlayerPositions[lastRolledPlayer];
        
//         mockPlayerPositions[lastRolledPlayer] = (startTile + totalRoll) % 40;
        
//         isAnimatingToken = true;
//         animatingPlayerIdx = lastRolledPlayer;
//         animationPath.clear();
//         hopProgress = 0.0f;
        
//         for (int i = 1; i <= totalRoll; ++i) {
//             animationPath.push_back((startTile + i) % 40);
//         }
        
//         startVisualPos = boardGame.getTokenPosition(startTile) + boardGame.computeTokenOffsetForStackIndex(animatingPlayerIdx);
//         int nextTile = animationPath.front();
//         targetVisualPos = boardGame.getTokenPosition(nextTile) + boardGame.computeTokenOffsetForStackIndex(animatingPlayerIdx);
        
//         playerDirections[animatingPlayerIdx] = getDirectionForTile(nextTile);
//     }

//     sf::Event event;
//     while (window.pollEvent(event)) {
//         if (event.type == sf::Event::Closed) window.close();

//         if (activePopup != nullptr) {
//             if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
//                 PopupResult res = activePopup->handleMouseClick(event.mouseButton.x, event.mouseButton.y);
                
//                 if (res != PopupResult::NONE) {
//                     if (res == PopupResult::AUCTION_PROPERTY) {
//                         rightSide.addHistoryEntry("Properti masuk ke pelelangan!");
                        
//                         int landedTile = mockPlayerPositions[lastRolledPlayer];
//                         std::string tName; int tPrice; bool isProp;
//                         getMockTileData(landedTile, tName, tPrice, isProp);
                        
//                         int starter = (lastRolledPlayer + 1) % 4;
//                         while (!mockPlayerActive[starter]) starter = (starter + 1) % 4;

//                         delete activePopup;
                        
//                         activePopup = new AuctionPopup(tName, 0, starter);
//                         activePopup->loadAssets();
                        
//                         auctionPassCount = 0; 
//                         continue; 
//                     } 
//                     else if (res == PopupResult::AUCTION_BID) {
//                         AuctionPopup* ap = dynamic_cast<AuctionPopup*>(activePopup);
//                         if (ap) {
//                             int newBid = (ap->getCurrentBid() == 0) ? 0 : ap->getCurrentBid() + 10;
//                             ap->updateBid(newBid, ap->getActiveBidderIdx());
//                             rightSide.addHistoryEntry("Pemain " + std::to_string(ap->getActiveBidderIdx() + 1) + " menawar $" + std::to_string(newBid));
//                             ap->nextTurn(mockPlayerActive);
//                             auctionPassCount = 0; 
//                         }
//                         continue; 
//                     } 
//                     else if (res == PopupResult::AUCTION_PASS) {
//                         AuctionPopup* ap = dynamic_cast<AuctionPopup*>(activePopup);
//                         if (ap) {
//                             rightSide.addHistoryEntry("Pemain " + std::to_string(ap->getActiveBidderIdx() + 1) + " PASS.");
//                             auctionPassCount++;
                            
//                             int activeCount = 0;
//                             for (bool isActive : mockPlayerActive) {
//                                 if (isActive) activeCount++;
//                             }

//                             if (auctionPassCount >= activeCount - 1 && ap->getHighBidderIdx() != -1) {
//                                 rightSide.addHistoryEntry("Lelang DIMENANGKAN oleh Pemain " + std::to_string(ap->getHighBidderIdx() + 1) + " seharga $" + std::to_string(ap->getCurrentBid()));
                                
//                                 int landedTile = mockPlayerPositions[lastRolledPlayer];
//                                 mockPropertyData[landedTile].ownerIndex = ap->getHighBidderIdx();
//                                 mockPropertyData[landedTile].level = 1; 

//                                 delete activePopup;
//                                 activePopup = nullptr;
//                                 continue;
//                             } 
//                             else if (auctionPassCount >= activeCount && ap->getHighBidderIdx() == -1) {
//                                 rightSide.addHistoryEntry("Lelang dibatalkan.");
//                                 delete activePopup;
//                                 activePopup = nullptr;
//                                 continue;
//                             } 
//                             else {
//                                 ap->nextTurn(mockPlayerActive);
//                             }
//                         }
//                         continue; 
//                     }
//                     else if (res == PopupResult::BUY_PROPERTY) {
//                         rightSide.addHistoryEntry("Property bought!");
//                         int landedTile = mockPlayerPositions[lastRolledPlayer];
//                         mockPropertyData[landedTile].ownerIndex = lastRolledPlayer; 
//                         mockPropertyData[landedTile].level = 1; 
//                     } else if (res == PopupResult::AUCTION_PROPERTY) {
//                         rightSide.addHistoryEntry("Property went to auction!");
//                     } else if (res == PopupResult::OK_CLOSE) {
//                         rightSide.addHistoryEntry("Pemain menutup popup.");
//                     } else if (res == PopupResult::TAX_PERCENTAGE) {
//                         rightSide.addHistoryEntry("Memilih bayar pajak 10%.");
//                     } else if (res == PopupResult::TAX_FLAT) {
//                         rightSide.addHistoryEntry("Memilih bayar pajak flat.");
//                     }
//                     else if (res == PopupResult::APPLY_FESTIVAL) {
//                         FestivalPopup* fp = dynamic_cast<FestivalPopup*>(activePopup);
//                         if (fp) {
//                             rightSide.addHistoryEntry("Festival diterapkan di properti index " + std::to_string(fp->getSelectedTileIndex()));
//                         }
//                     }
//                     else if (res == PopupResult::CLOSE_POPUP) {
//                     } 
//                     else if (res == PopupResult::MORTGAGE_PROPERTY) {
//                         MortgagePopup* gp = dynamic_cast<MortgagePopup*>(activePopup);
//                         if (gp) {
//                             int tileIdx = gp->getSelectedTileIndex();
//                             rightSide.addHistoryEntry("Properti index " + std::to_string(tileIdx) + " berhasil digadaikan.");
//                         }
//                     } 
//                     else if (res == PopupResult::SELL_BUILDINGS) {
//                         MortgagePopup* gp = dynamic_cast<MortgagePopup*>(activePopup);
//                         if (gp) {
//                             int tileIdx = gp->getSelectedTileIndex();
//                             rightSide.addHistoryEntry("Bangunan di color group index " + std::to_string(tileIdx) + " terjual.");
//                         }
//                     }
                    
//                     if (res != PopupResult::AUCTION_BID && res != PopupResult::AUCTION_PASS && res != PopupResult::AUCTION_PROPERTY) {
//                         delete activePopup; 
//                         activePopup = nullptr; 
//                     }
//                 }
//             }
//             continue;
//         }

//         // --- DEBUG KEYS (SPASI: MAJU, BACKSPACE: MUNDUR) ---
//         if (event.type == sf::Event::KeyPressed && appState == AppState::PLAYING && !isAnimatingToken) {
//             if (event.key.code == sf::Keyboard::Space) {
//                 lastRolledPlayer = currentTurn; 
//                 int startTile = mockPlayerPositions[currentTurn];
//                 mockPlayerPositions[currentTurn] = (startTile + 1) % 40;
                
//                 isAnimatingToken = true;
//                 animatingPlayerIdx = currentTurn;
//                 animationPath.clear();
//                 hopProgress = 0.0f;
//                 animationPath.push_back(mockPlayerPositions[currentTurn]);
                
//                 startVisualPos = boardGame.getTokenPosition(startTile) + boardGame.computeTokenOffsetForStackIndex(animatingPlayerIdx);
//                 targetVisualPos = boardGame.getTokenPosition(animationPath.front()) + boardGame.computeTokenOffsetForStackIndex(animatingPlayerIdx);
//                 playerDirections[animatingPlayerIdx] = getDirectionForTile(animationPath.front());
//             }
//             else if (event.key.code == sf::Keyboard::BackSpace) {
//                 lastRolledPlayer = currentTurn; 
//                 int startTile = mockPlayerPositions[currentTurn];
//                 mockPlayerPositions[currentTurn] = (startTile + 39) % 40; 
                
//                 isAnimatingToken = true;
//                 animatingPlayerIdx = currentTurn;
//                 animationPath.clear();
//                 hopProgress = 0.0f;
//                 animationPath.push_back(mockPlayerPositions[currentTurn]);
                
//                 startVisualPos = boardGame.getTokenPosition(startTile) + boardGame.computeTokenOffsetForStackIndex(animatingPlayerIdx);
//                 targetVisualPos = boardGame.getTokenPosition(animationPath.front()) + boardGame.computeTokenOffsetForStackIndex(animatingPlayerIdx);
//                 playerDirections[animatingPlayerIdx] = getDirectionForTile(animationPath.front());
//             }
//         }

//         // Kunci Popups (G dan A)
//         if (event.type == sf::Event::KeyPressed && appState == AppState::PLAYING && activePopup == nullptr) {
//             if (event.key.code == sf::Keyboard::G) {
//                 std::vector<MortgageItem> mockGadaiList;
//                 mockGadaiList.push_back({38, "Jakarta (JKT)", "BIRU TUA", 200, false});
//                 mockGadaiList.push_back({6, "Stasiun Gambir (GBR)", "STASIUN", 100, false});
//                 mockGadaiList.push_back({32, "Bandung (BDG)", "HIJAU", 150, true});
//                 activePopup = new MortgagePopup(mockGadaiList);
//                 activePopup->loadAssets();
//             }

//             if (event.key.code == sf::Keyboard::A) {
//                 PropertyAktaData mockupData;
//                 mockupData.name = "JAKARTA (JKT)";
//                 mockupData.colorGroupName = "BIRU TUA";
//                 mockupData.colorCode = sf::Color(0, 51, 153);
//                 mockupData.price = 400;
//                 mockupData.mortgageValue = 200;
//                 mockupData.baseRent = 50;
//                 mockupData.rentWithHouses = {200, 600, 1400, 1700};
//                 mockupData.rentWithHotel = 2000;
//                 mockupData.houseCost = 200;
//                 mockupData.hotelCost = 200;
                
//                 int ownerIdx = mockPropertyData[38].ownerIndex; 
//                 if (ownerIdx != -1) mockupData.ownerName = rightSide.getPlayerName(ownerIdx);
//                 else mockupData.ownerName = "";

//                 activePopup = new PropertyAktaPopup(mockupData);
//                 activePopup->loadAssets();
//             }
//         }

//         // --- INPUT NORMAL ---
//         if (event.type == sf::Event::MouseMoved) {
//             float mouseX = static_cast<float>(event.mouseMove.x);
//             float mouseY = static_cast<float>(event.mouseMove.y);
//             isMenuBarVisible = (mouseY <= 40.f || menu.isMenuDropdownOpen());
//             if (isMenuBarVisible) menu.handleMouseMove(mouseX, mouseY);
//         }

//         if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
//             float mouseX = static_cast<float>(event.mouseButton.x);
//             float mouseY = static_cast<float>(event.mouseButton.y);

//             if (isMenuBarVisible || menu.isMenuDropdownOpen()) {
//                 MenuAction action = menu.handleMouseClick(mouseX, mouseY);
//                 if (action == MenuAction::QUIT) window.close();
//                 else if (action == MenuAction::NEW_GAME) {
//                     appState = AppState::SETUP;
//                     rightSide.resetSetup();
//                 }
//             }

//             if (appState != AppState::IDLE && mouseX >= 800.f) {
//                 rightSide.handleMouseClick(mouseX, mouseY);
//                 if (rightSide.pollStartGameClicked()) {
//                     appState = AppState::PLAYING;
//                     mockPlayerActive = rightSide.getActivePlayers(); 
//                     currentTurn = rightSide.getFirstTurnPlayerIndex();
//                     if (!mockPlayerActive[currentTurn]) {
//                         currentTurn = 0;
//                         while (!mockPlayerActive[currentTurn] && currentTurn < 4) currentTurn++;
//                     }
//                     rightSide.setActivePlayerTurn(currentTurn);
//                 }
//             }
//         }

//         if (event.type == sf::Event::TextEntered && appState == AppState::SETUP) {
//             rightSide.handleTextInput(event.text.unicode);
//         }
//     }
// }

// bool SFMLView::isWindowOpen() const {
//     return window.isOpen();
// }

// void SFMLView::updateBoardState(GameContext G) {
//     // TODO: Update posisi karakter berdasarkan data dari G
// }

// void SFMLView::showDiceAnimation(int dice1, int dice2) {
//     // TODO: Mainkan animasi dadu sesuai angka dari backend
// }

// void SFMLView::triggerPopup(const string& popupType, Tile* tileData) {
//     // TODO: Munculkan popup sesuai tipe
// }

// void SFMLView::closeUI() {
//     window.close();
// }

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

        // 1. TANGKAP INPUT POPUP
        if (activePopup != nullptr) {
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                PopupResult res = activePopup->handleMouseClick(event.mouseButton.x, event.mouseButton.y);
                
                // Jika tombol di dalam popup ditekan
                if (res != PopupResult::NONE) {
                    if (res == PopupResult::BUY_PROPERTY) lastPopupResponse = 1;
                    else if (res == PopupResult::AUCTION_PROPERTY) lastPopupResponse = 2;
                    else if (res == PopupResult::OK_CLOSE) lastPopupResponse = 0;
                    else lastPopupResponse = 99; // Kode aksi lainnya

                    // --- TODO UNTUK BACKEND ---
                    // Nanti di sini kita hanya menyimpan 'res' ke sebuah variabel (misal: lastPopupInteraction = res;)
                    // Lalu GameEngine akan membacanya dan menjalankan logika game.
                    // GUI TIDAK BOLEH lagi mengeksekusi logika lelang/beli tanah di sini!
                    
                    if (res != PopupResult::AUCTION_BID && res != PopupResult::AUCTION_PASS) {
                        delete activePopup; 
                        activePopup = nullptr; 
                    }
                }
            }
            continue; // Abaikan klik lain jika popup sedang terbuka
        }

        // 2. TANGKAP KLIK MENU / SIDE PANEL
        if (event.type == sf::Event::MouseMoved) {
            float mouseX = static_cast<float>(event.mouseMove.x);
            float mouseY = static_cast<float>(event.mouseMove.y);
            isMenuBarVisible = (mouseY <= 40.f || menu.isMenuDropdownOpen());
            if (isMenuBarVisible) menu.handleMouseMove(mouseX, mouseY);
        }

        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            float mouseX = static_cast<float>(event.mouseButton.x);
            float mouseY = static_cast<float>(event.mouseButton.y);

            if (isMenuBarVisible || menu.isMenuDropdownOpen()) {
                MenuAction action = menu.handleMouseClick(mouseX, mouseY);
                if (action == MenuAction::QUIT) window.close();
                // Logic New Game diserahkan ke backend nanti
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
void SFMLView::updateBoardState(GameContext G) {
    // TODO: Update posisi karakter berdasarkan data dari G
}

void SFMLView::showDiceAnimation(int dice1, int dice2) {
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

    // std::string tileName = (tileData != nullptr) ? tileData->getName() : "Petak Misterius";

    // if (popupType == "BUY_PROPERTY") {
    //     int price = 200;
        
    //     activePopup = new PropertyPopup(tileName, price, "Pemain Aktif");
        
    // } 
    // else if (popupType == "PAY_RENT") {
    //     activePopup = new AlertPopup("Bayar Sewa", "Kamu mendarat di properti orang!", "Pemain", "Bayar Sekarang");
        
    // } 
    // else if (popupType == "AUCTION") {
    //     activePopup = new AuctionPopup(tileName, 0, currentTurnVisual);
        
    // }

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

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return CommandType::UNKNOWN_COMMAND;
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
                float mouseX = static_cast<float>(event.mouseButton.x);
                float mouseY = static_cast<float>(event.mouseButton.y);

                if (mouseX >= 800.f) { 
                    rightSide.handleMouseClick(mouseX, mouseY);
                    
                }
            }
        }

        renderBoard();
    }

    return CommandType::UNKNOWN_COMMAND;
}