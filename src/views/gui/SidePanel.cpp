#include "SidePanel.hpp"
#include <iostream>
#include <algorithm>
#include <sstream>

namespace {
std::vector<std::string> wrapTextToWidth(const sf::Font& font, unsigned int size, const std::string& text, float maxWidth) {
    std::vector<std::string> lines;
    if (text.empty()) {
        lines.push_back("");
        return lines;
    }

    sf::Text measure;
    measure.setFont(font);
    measure.setCharacterSize(size);

    std::istringstream iss(text);
    std::string word;
    std::string current;

    auto textWidth = [&](const std::string& s) {
        measure.setString(s);
        return measure.getLocalBounds().width;
    };

    while (iss >> word) {
        std::string candidate = current.empty() ? word : current + " " + word;
        if (textWidth(candidate) <= maxWidth) {
            current = candidate;
            continue;
        }

        if (!current.empty()) {
            lines.push_back(current);
            current.clear();
        }

        if (textWidth(word) <= maxWidth) {
            current = word;
            continue;
        }

        std::string chunk;
        for (char c : word) {
            std::string nextChunk = chunk + c;
            if (!chunk.empty() && textWidth(nextChunk) > maxWidth) {
                lines.push_back(chunk);
                chunk = std::string(1, c);
            } else {
                chunk = nextChunk;
            }
        }
        current = chunk;
    }

    if (!current.empty()) {
        lines.push_back(current);
    }

    return lines;
}
}

SidePanel::SidePanel() {
    currentState = PanelState::PLAYER_SETUP;
    selectedSetupCharacter = 0;
    activePlayerIndex = 0;
    firstTurnPlayerIndex = 0;
    
    isDiceRolling = false;
    flagStartClicked = false;
    flagRollFinished = false;
    flagRollClicked = false;
    flagSkillCardsClicked = false;

    diceValues = {1, 1};
    cardInfoText = "No cards played yet.";

    isPlayerTextureLoaded.fill(false);
    isDiceTextureLoaded.fill(false);
    isPlayerEnabled.fill(false); 
    // isPlayerEnabled[0] = true;
    isNameEdited.fill(false);

    currentCommandInput = "";
    flagCommandReady = false;

    for (int i = 0; i < 4; ++i) {
        playerNames[i] = "Player " + std::to_string(i + 1);
    }
}

void SidePanel::resetSetup() {
    currentState = PanelState::PLAYER_SETUP;
    selectedSetupCharacter = 0;
    firstTurnPlayerIndex = 0;
    isPlayerEnabled.fill(false);
    isNameEdited.fill(false);    
    gameHistory.clear();
    for (int i = 0; i < 4; ++i) {
        playerNames[i] = "Player " + std::to_string(i + 1);
    }
}

void SidePanel::loadAssets() {
    if (!mainFont.loadFromFile("assets/Roboto-Black.ttf")) {
        std::cerr << "ERROR: Failed to load font in SidePanel.\n";
    }
    for (int i = 0; i < 4; ++i) {
        std::string path = "assets/player" + std::to_string(i + 1) + ".png";
        isPlayerTextureLoaded[i] = playerTextures[i].loadFromFile(path);
        if (isPlayerTextureLoaded[i]) playerTextures[i].setSmooth(true);
    }
    for (int i = 0; i < 6; ++i) {
        std::string path = "assets/dice" + std::to_string(i + 1) + ".png";
        isDiceTextureLoaded[i] = diceTextures[i].loadFromFile(path);
        if (isDiceTextureLoaded[i]) diceTextures[i].setSmooth(true);
    }
}

void SidePanel::fitToLayout(sf::FloatRect containerArea) {
    panelArea = containerArea;
}

// --- Method Utama ---
void SidePanel::update() {
    if (isDiceRolling) {
        if (diceTickClock.getElapsedTime().asSeconds() > 0.07f) {
            diceValues[0] = (rand() % 6) + 1;
            diceValues[1] = (rand() % 6) + 1;
            diceTickClock.restart();
        }
        
        if (diceRollClock.getElapsedTime().asSeconds() > 0.95f) {
            isDiceRolling = false;
            diceValues[0] = targetDiceValues[0];
            diceValues[1] = targetDiceValues[1];
        }
    }
}

void SidePanel::render(sf::RenderWindow& window) {
    draw3DPanel(window, panelArea, sf::Color(192, 192, 192), false);
    if (currentState == PanelState::PLAYER_SETUP) renderSetupState(window);
    else renderInGameState(window);
}

void SidePanel::setPlayerBalance(int index, int balance) {
    if (index >= 0 && index < 4) playerBalances[index] = balance;
}

// --- Input Handler
void SidePanel::handleMouseClick(float mouseX, float mouseY) {
    if (currentState == PanelState::PLAYER_SETUP) {
        for (int i = 0; i < 4; ++i) {
            if (containsPoint(avatarSelectRects[i], mouseX, mouseY)) {
                selectedSetupCharacter = i;
                isPlayerEnabled[i] = true;
                return;
            }
        }
        if (containsPoint(takeMoneyButtonRect, mouseX, mouseY)) {
            isPlayerEnabled[selectedSetupCharacter] = false;
        }
        if (containsPoint(giveDiceButtonRect, mouseX, mouseY)) {
            firstTurnPlayerIndex = selectedSetupCharacter;
            isPlayerEnabled[selectedSetupCharacter] = true;
        }
        if (containsPoint(startGameButtonRect, mouseX, mouseY)) {
            bool hasPlayer = false;
            for(bool en : isPlayerEnabled) if(en) hasPlayer = true;
            
            if (hasPlayer) {
                currentState = PanelState::IN_GAME;
                flagStartClicked = true;
                addHistoryEntry("Game Started!");
            }
        }

    } else if (currentState == PanelState::IN_GAME) {
        if (containsPoint(rollDiceButtonRect, mouseX, mouseY)) {
            flagRollClicked = true;
            addHistoryEntry("Throwing Dice...");
        } else if (containsPoint(skillCardsButtonRect, mouseX, mouseY)) {
            flagSkillCardsClicked = true;
        }
    }
}

void SidePanel::handleTextInput(unsigned int unicode) {
    if (currentState == PanelState::PLAYER_SETUP) {
        std::string& name = playerNames[selectedSetupCharacter];
        if (unicode == 8) { // Backspace
            if (!isNameEdited[selectedSetupCharacter]) {
                name = ""; isNameEdited[selectedSetupCharacter] = true;
            } else if (!name.empty()) name.pop_back();
        } else if (unicode >= 32 && unicode <= 126 && name.size() < 12) {
            if (!isNameEdited[selectedSetupCharacter]) {
                name = ""; isNameEdited[selectedSetupCharacter] = true;
            }
            name += static_cast<char>(unicode);
        }
    }
    else if (currentState == PanelState::IN_GAME) {
        if (unicode == 8) { // Tombol Backspace
            if (!currentCommandInput.empty()) currentCommandInput.pop_back();
        } else if (unicode == 13) { // Tombol Enter
            lastReadyCommand = currentCommandInput;
            currentCommandInput = ""; // Kosongkan kolom setelah di-enter
            flagCommandReady = true;  
        } else if (unicode >= 32 && unicode <= 126 && currentCommandInput.size() < 22) {
            currentCommandInput += static_cast<char>(unicode);
        }
    }
}

// --- Getter & Signals ---
bool SidePanel::pollRollButtonClicked() {
    if (flagRollClicked) { 
        flagRollClicked = false; 
        return true; 
    } 
    return false;
}

bool SidePanel::pollStartGameClicked() {
    if (flagStartClicked) { 
        flagStartClicked = false; 
        return true; 
    } 
    return false;
}

bool SidePanel::pollSkillCardsButtonClicked() {
    if (flagSkillCardsClicked) {
        flagSkillCardsClicked = false;
        return true;
    }
    return false;
}

bool SidePanel::pollRollDiceFinished(int& outDice1, int& outDice2) {
    if (flagRollFinished) { 
        flagRollFinished = false; 
        outDice1 = diceValues[0]; 
        outDice2 = diceValues[1]; 
        return true; 
    } 
    return false;
}

void SidePanel::addHistoryEntry(const std::string& entry) { gameHistory.push_back(entry); }
void SidePanel::setPanelState(PanelState state) { currentState = state; }
void SidePanel::setActivePlayerTurn(int index) { activePlayerIndex = index; }
void SidePanel::setPlayerData(int index, const std::string& name, bool isActive) {
    if (index >= 0 && index < 4) {
        playerNames[index] = name;
        isPlayerEnabled[index] = isActive;
    }
}
std::array<bool, 4> SidePanel::getActivePlayers() const { return isPlayerEnabled; }

int SidePanel::getFirstTurnPlayerIndex() const {
    return firstTurnPlayerIndex;
}

std::string SidePanel::getPlayerName(int index) const {
    if (index >= 0 && index < 4) return playerNames[index];
    return "Unknown";
}

// --- Helper ---
bool SidePanel::containsPoint(const sf::FloatRect& rect, float x, float y) const {
    return x >= rect.left && x <= rect.left + rect.width && y >= rect.top && y <= rect.top + rect.height;
}

sf::Text SidePanel::createText(const std::string& text, float x, float y, unsigned int size, const sf::Color& color) const {
    sf::Text sfText; sfText.setFont(mainFont); sfText.setString(text);
    sfText.setPosition(x, y); sfText.setCharacterSize(size); sfText.setFillColor(color);
    return sfText;
}

void SidePanel::draw3DPanel(sf::RenderWindow& window, sf::FloatRect rect, sf::Color baseColor, bool isSunken) const {
    sf::RectangleShape panel(sf::Vector2f(rect.width, rect.height));
    panel.setPosition(rect.left, rect.top); panel.setFillColor(baseColor);
    window.draw(panel);
    sf::Color colorTopLeft = isSunken ? sf::Color(100, 100, 100) : sf::Color::White;
    sf::Color colorBottomRight = isSunken ? sf::Color::White : sf::Color(100, 100, 100);
    
    sf::RectangleShape hlTop(sf::Vector2f(rect.width, 2.f)); hlTop.setPosition(rect.left, rect.top); hlTop.setFillColor(colorTopLeft);
    sf::RectangleShape hlLeft(sf::Vector2f(2.f, rect.height)); hlLeft.setPosition(rect.left, rect.top); hlLeft.setFillColor(colorTopLeft);
    sf::RectangleShape shBottom(sf::Vector2f(rect.width, 2.f)); shBottom.setPosition(rect.left, rect.top + rect.height - 2.f); shBottom.setFillColor(colorBottomRight);
    sf::RectangleShape shRight(sf::Vector2f(2.f, rect.height)); shRight.setPosition(rect.left + rect.width - 2.f, rect.top); shRight.setFillColor(colorBottomRight);
    
    window.draw(hlTop); window.draw(hlLeft); window.draw(shBottom); window.draw(shRight);
}

void SidePanel::renderSetupState(sf::RenderWindow& window) {
    float startX = panelArea.left + 8.f; 
    float currentY = panelArea.top + 8.f; 
    float innerW = panelArea.width - 16.f;

    draw3DPanel(window, sf::FloatRect(startX, currentY, innerW, 30.f), sf::Color(40, 80, 200), false);
    window.draw(createText("Player Setup", startX + (innerW/2.f) - 55.f, currentY + 4.f, 18, sf::Color::White));
    currentY += 30.f;

    sf::FloatRect bgRect(startX, currentY, innerW, panelArea.height - 120.f);
    draw3DPanel(window, bgRect, sf::Color(228, 242, 228), true); 

    float tabX = startX + 10.f;
    float tabY = currentY + 20.f;
    
    sf::RectangleShape tabTop(sf::Vector2f(140.f, 32.f));
    tabTop.setPosition(tabX, tabY);
    tabTop.setFillColor(sf::Color::White);
    tabTop.setOutlineThickness(2.f);
    tabTop.setOutlineColor(sf::Color(150, 150, 150));
    window.draw(tabTop);
    
    sf::RectangleShape tabBody(sf::Vector2f(innerW - 20.f, 120.f));
    tabBody.setPosition(tabX, tabY + 30.f); 
    tabBody.setFillColor(sf::Color::White);
    tabBody.setOutlineThickness(2.f);
    tabBody.setOutlineColor(sf::Color(150, 150, 150));
    window.draw(tabBody);

    sf::RectangleShape hideLine(sf::Vector2f(136.f, 4.f));
    hideLine.setPosition(tabX + 2.f, tabY + 30.f);
    hideLine.setFillColor(sf::Color::White);
    window.draw(hideLine);

    std::string currentName = playerNames[selectedSetupCharacter];
    sf::Text nameText = createText(currentName, tabX + 10.f, tabY + 2.f, 18, sf::Color::Black);
    
    if (!isNameEdited[selectedSetupCharacter]) {
        sf::FloatRect bounds = nameText.getGlobalBounds();
        sf::RectangleShape hl(sf::Vector2f(bounds.width + 10.f, bounds.height + 8.f));
        hl.setPosition(bounds.left - 4.f, bounds.top - 2.f);
        hl.setFillColor(sf::Color(51, 153, 255)); 
        window.draw(hl);
        nameText.setFillColor(sf::Color::White);
    }
    window.draw(nameText);

    if (isPlayerTextureLoaded[selectedSetupCharacter]) {
        sf::Sprite bigAvatar(playerTextures[selectedSetupCharacter]);
        bigAvatar.setScale(0.12f, 0.12f);
        bigAvatar.setPosition(tabX + (tabBody.getSize().x / 2.f) - 30.f, tabY + 35.f);
        window.draw(bigAvatar);
    }

    takeMoneyButtonRect = sf::FloatRect(tabX + 15.f, tabY + 95.f, 110.f, 35.f);
    giveDiceButtonRect = sf::FloatRect(tabX + 135.f, tabY + 95.f, 110.f, 35.f);
    
    bool isActive = isPlayerEnabled[selectedSetupCharacter];
    draw3DPanel(window, takeMoneyButtonRect, sf::Color(220, 220, 220), isActive); 
    draw3DPanel(window, giveDiceButtonRect, sf::Color(220, 220, 220), !isActive);

    window.draw(createText("Take [ M ]", takeMoneyButtonRect.left + 15.f, takeMoneyButtonRect.top + 8.f, 16, sf::Color::Black));
    window.draw(createText("Give [ D ]", giveDiceButtonRect.left + 15.f, giveDiceButtonRect.top + 8.f, 16, sf::Color::Black));

    currentY += 180.f;

    float gridStartX = startX + 30.f;
    for (int i = 0; i < 4; ++i) {
        float ax = gridStartX + (i % 2) * 110.f;
        float ay = currentY + (i / 2) * 90.f;
        avatarSelectRects[i] = sf::FloatRect(ax, ay, 60.f, 60.f);
        
        if (i == selectedSetupCharacter) {
            sf::RectangleShape outline(sf::Vector2f(64.f, 64.f));
            outline.setPosition(ax - 2.f, ay - 2.f);
            outline.setFillColor(sf::Color::Transparent);
            outline.setOutlineThickness(2.f);
            outline.setOutlineColor(sf::Color::Black);
            window.draw(outline);
        }

        if (isPlayerEnabled[i]) {
            sf::RectangleShape moneyBox(sf::Vector2f(45.f, 18.f));
            moneyBox.setPosition(ax + 7.f, ay + 65.f);
            moneyBox.setFillColor(sf::Color(255, 204, 0)); 
            moneyBox.setOutlineThickness(1.f);
            moneyBox.setOutlineColor(sf::Color::Black);
            window.draw(moneyBox);
            window.draw(createText("$1.5k", ax + 11.f, ay + 65.f, 12, sf::Color::Black));

            if (firstTurnPlayerIndex == i && isDiceTextureLoaded[0]) {
                sf::Sprite tinyDice(diceTextures[0]); 
                tinyDice.setScale(0.04f, 0.04f); 
                tinyDice.setPosition(ax + 55.f, ay + 64.f); 
                window.draw(tinyDice);
            }
        }

        if (isPlayerTextureLoaded[i]) {
            sf::Sprite av(playerTextures[i]);
            av.setScale(0.08f, 0.08f);
            sf::FloatRect bounds = av.getGlobalBounds();
            // Rumus tengah: (Panjang Kotak - Panjang Gambar) / 2
            float centerX = ax + (60.f - bounds.width) / 2.f;
            float centerY = ay + (60.f - bounds.height) / 2.f;
            av.setPosition(centerX, centerY);
            window.draw(av);
        }
    }

    float startBtnY = panelArea.top + panelArea.height - 60.f;
    startGameButtonRect = sf::FloatRect(startX, startBtnY, innerW, 50.f);
    draw3DPanel(window, startGameButtonRect, sf::Color(40, 80, 200), false);
    window.draw(createText("START GAME", startX + (innerW/2.f) - 55.f, startBtnY + 12.f, 20, sf::Color::White));
}

void SidePanel::renderInGameState(sf::RenderWindow& window) {
    float startX = panelArea.left + 8.f; 
    float currentY = panelArea.top + 8.f; 
    float innerW = panelArea.width - 16.f;

    draw3DPanel(window, sf::FloatRect(startX, currentY, innerW, 30.f), sf::Color(40, 80, 200), false);
    window.draw(createText("Players", startX + (innerW/2.f) - 35.f, currentY + 4.f, 18, sf::Color::White));
    currentY += 30.f;

    sf::FloatRect bgRect(startX, currentY, innerW, panelArea.height - 46.f);
    draw3DPanel(window, bgRect, sf::Color(228, 242, 228), true);

    float tabX = startX + 10.f;
    float tabY = currentY + 15.f;
    
    sf::RectangleShape tabTop(sf::Vector2f(140.f, 32.f));
    tabTop.setPosition(tabX, tabY);
    tabTop.setFillColor(sf::Color::White);
    tabTop.setOutlineThickness(2.f);
    tabTop.setOutlineColor(sf::Color(150, 150, 150));
    window.draw(tabTop);
    
    sf::RectangleShape tabBody(sf::Vector2f(innerW - 20.f, 120.f));
    tabBody.setPosition(tabX, tabY + 30.f);
    tabBody.setFillColor(sf::Color::White);
    tabBody.setOutlineThickness(2.f);
    tabBody.setOutlineColor(sf::Color(150, 150, 150));
    window.draw(tabBody);

    sf::RectangleShape hideLine(sf::Vector2f(136.f, 4.f));
    hideLine.setPosition(tabX + 2.f, tabY + 30.f);
    hideLine.setFillColor(sf::Color::White);
    window.draw(hideLine);

    std::string currentName = playerNames[activePlayerIndex];
    window.draw(createText(currentName, tabX + 10.f, tabY + 6.f, 18, sf::Color::Black));

    if (isPlayerTextureLoaded[activePlayerIndex]) {
        sf::Sprite bigAvatar(playerTextures[activePlayerIndex]);
        bigAvatar.setScale(0.12f, 0.12f);
        bigAvatar.setPosition(tabX + tabBody.getSize().x - 70.f, tabY + 40.f);
        window.draw(bigAvatar);
    }

    std::string moneyStr = "M" + std::to_string(playerBalances[activePlayerIndex]);
    sf::FloatRect cashRect(tabX + 15.f, tabY + 65.f, 150.f, 35.f);
    draw3DPanel(window, cashRect, sf::Color(240, 240, 240), true);
    window.draw(createText("Cash", cashRect.left + 50.f, cashRect.top + 2.f, 14, sf::Color::Black));
    window.draw(createText(moneyStr, cashRect.left + 42.f, cashRect.top + 16.f, 16, sf::Color(28, 35, 64)));  
      
    currentY += 170.f;

    // Dadu
    sf::FloatRect actionRect(startX + 10.f, currentY, innerW - 20.f, 80.f);
    draw3DPanel(window, actionRect, sf::Color(210, 210, 210), false);

    // Tombol Roll 
    rollDiceButtonRect = sf::FloatRect(actionRect.left + 15.f, actionRect.top + 10.f, 80.f, 30.f);
    draw3DPanel(window, rollDiceButtonRect, isDiceRolling ? sf::Color(150, 150, 150) : sf::Color(220, 220, 220), isDiceRolling);
    window.draw(createText("ROLL", rollDiceButtonRect.left + 22.f, rollDiceButtonRect.top + 6.f, 14, sf::Color::Black));
    
    // Tombol Skill Cards
    skillCardsButtonRect = sf::FloatRect(actionRect.left + 15.f, actionRect.top + 45.f, 80.f, 30.f);
    draw3DPanel(window, skillCardsButtonRect, sf::Color(220, 220, 220), false);
    window.draw(createText("SKILLS", skillCardsButtonRect.left + 15.f, skillCardsButtonRect.top + 6.f, 14, sf::Color::Black));
    
    for (int d = 0; d < 2; ++d) {
        int faceIndex = diceValues[d] - 1;
        if (isDiceTextureLoaded[faceIndex]) {
            sf::Sprite diceSp(diceTextures[faceIndex]);
            
            float targetSize = 46.f; 
            float texW = diceTextures[faceIndex].getSize().x;
            float texH = diceTextures[faceIndex].getSize().y;
            float scale = targetSize / std::max(texW, texH);
            
            diceSp.setScale(scale, scale);
            
            float diceY = actionRect.top + (actionRect.height - targetSize) / 2.f;
            
            float diceX = actionRect.left + 125.f + (d * (targetSize + 15.f));
            
            diceSp.setPosition(diceX, diceY);
            window.draw(diceSp);
        }
    }
    
    currentY += 95.f;

    //  Game History 
    draw3DPanel(window, sf::FloatRect(startX + 10.f, currentY, innerW - 20.f, 25.f), sf::Color(100, 140, 210), false);
    window.draw(createText("Game History", startX + 20.f, currentY + 3.f, 15, sf::Color::White));
    currentY += 25.f;

    const float cmdAreaHeight = 34.f;
    const float spacingAboveCmd = 8.f;
    const float bottomMargin = 8.f;
    float remainingHeight = (panelArea.top + panelArea.height) - currentY - cmdAreaHeight - spacingAboveCmd - bottomMargin;
    if (remainingHeight < 80.f) {
        remainingHeight = 80.f;
    }

    sf::FloatRect histRect(startX + 10.f, currentY, innerW - 20.f, remainingHeight);
    draw3DPanel(window, histRect, sf::Color::White, true);

    const unsigned int historyFontSize = 12;
    const float historyLineHeight = 16.f;
    const float textLeftPadding = 8.f;
    const float textTopBottomPadding = 6.f;
    const float maxTextWidth = histRect.width - (2.f * textLeftPadding);

    std::vector<std::string> wrappedHistoryLines;
    wrappedHistoryLines.reserve(gameHistory.size() * 2);
    for (const std::string& entry : gameHistory) {
        std::vector<std::string> wrapped = wrapTextToWidth(mainFont, historyFontSize, "> " + entry, maxTextWidth);
        wrappedHistoryLines.insert(wrappedHistoryLines.end(), wrapped.begin(), wrapped.end());
    }

    int maxVisibleLines = static_cast<int>((histRect.height - (2.f * textTopBottomPadding)) / historyLineHeight);
    if (maxVisibleLines < 1) {
        maxVisibleLines = 1;
    }

    int totalLines = static_cast<int>(wrappedHistoryLines.size());
    int startLine = std::max(0, totalLines - maxVisibleLines);
    int visibleCount = totalLines - startLine;
    float textY = histRect.top + histRect.height - textTopBottomPadding - (visibleCount * historyLineHeight);

    for (int i = startLine; i < totalLines; ++i) {
        window.draw(createText(wrappedHistoryLines[i], histRect.left + textLeftPadding, textY, historyFontSize, sf::Color(40, 40, 40)));
        textY += historyLineHeight;
    }

    currentY += histRect.height + 8.f;

    // Input Command
    window.draw(createText("Cmd:", startX + 10.f, currentY + 6.f, 16, sf::Color::Black));
    sf::FloatRect inputRect(startX + 50.f, currentY, innerW - 60.f, 30.f);
    draw3DPanel(window, inputRect, sf::Color::White, true);
    
    std::string displayCmd = currentCommandInput;
    if (static_cast<int>(diceTickClock.getElapsedTime().asSeconds() * 2) % 2 == 0) displayCmd += "_";
    window.draw(createText(displayCmd, inputRect.left + 8.f, inputRect.top + 6.f, 15, sf::Color::Black));
}

void SidePanel::setDiceResult(int d1, int d2) {
    targetDiceValues[0] = d1;
    targetDiceValues[1] = d2;

    isDiceRolling = true;
    diceRollClock.restart();
    diceTickClock.restart();
}

std::string SidePanel::pollCommandString() {
    if (flagCommandReady) {
        flagCommandReady = false;
        return lastReadyCommand;
    }
    return "";
}
