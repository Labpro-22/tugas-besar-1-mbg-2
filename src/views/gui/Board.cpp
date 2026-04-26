#include "Board.hpp"
#include <iostream>
#include <algorithm>
#include <array>

namespace {
    const float kCornerRatio = 1.75f;
    const int kTotalTiles = 40;
}

// --- Board ---
Board::Board() : boardDrawArea(0.f, 0.f, 0.f, 0.f), boardTextureLoaded(false) {
    tileCenters.resize(kTotalTiles);
    showDebugIndices = true;

    playerColorNames = {"Brown", "Green", "Red", "Blue"};
}

void Board::loadAssets() {
    if (!boardTexture.loadFromFile("assets/Nimonsy.png")) {
        std::cerr << "ERROR: Failed to load assets/Nimonsy.png\n";
        boardTextureLoaded = false;
        return;
    }

    boardTexture.setSmooth(true);
    boardTextureLoaded = true;

    for (int i = 0; i < 4; ++i) {
        std::string folder = "assets/player" + std::to_string(i + 1) + "/";
        
        bool f = playerTextures[i][PlayerDirection::Forward].loadFromFile(folder + "NimonForward.png");
        bool b = playerTextures[i][PlayerDirection::Back].loadFromFile(folder + "NimonBack.png");
        bool l = playerTextures[i][PlayerDirection::Left].loadFromFile(folder + "NimonLeft.png");
        bool r = playerTextures[i][PlayerDirection::Right].loadFromFile(folder + "NimonRight.png");
        
        isPlayerTexLoaded[i] = (f && b && l && r);
        
        if (isPlayerTexLoaded[i]) {
            playerTextures[i][PlayerDirection::Forward].setSmooth(true);
            playerTextures[i][PlayerDirection::Back].setSmooth(true);
            playerTextures[i][PlayerDirection::Left].setSmooth(true);
            playerTextures[i][PlayerDirection::Right].setSmooth(true);
        }
    }

    for (const std::string& color : playerColorNames) {
        // Load Level 1-4 (Houses)
        for (int i = 1; i <= 4; ++i) {
            std::string key = color + std::to_string(i);
            buildingTextures[key].loadFromFile("assets/property/" + key + ".png");
            buildingTextures[key].setSmooth(true);
        }
        // Load Hotel
        std::string hotelKey = color + "Hotel";
        buildingTextures[hotelKey].loadFromFile("assets/property/" + hotelKey + ".png");
        buildingTextures[hotelKey].setSmooth(true);
    }

    if (!debugFont.loadFromFile("assets/Roboto-Black.ttf")) { 
        std::cerr << "[ERROR] Failed to load font!\n";
    }
}

// --- Layout ---
void Board::fitToLayout(sf::FloatRect containerArea) {
    if (!boardTextureLoaded) return;

    const sf::Vector2u textureSize = boardTexture.getSize();
    if (textureSize.x == 0 || textureSize.y == 0) return;

    const float textureWidth = static_cast<float>(textureSize.x);
    const float textureHeight = static_cast<float>(textureSize.y);

    const float availableWidth = containerArea.width;
    const float availableHeight = containerArea.height;

    const float uniformScale = std::min(availableWidth / textureWidth, availableHeight / textureHeight);
    const float drawWidth = textureWidth * uniformScale;
    const float drawHeight = textureHeight * uniformScale;

    const float drawLeft = containerArea.left + (containerArea.width - drawWidth) / 2.f;
    const float drawTop = containerArea.top + (containerArea.height - drawHeight) / 2.f;
    
    boardDrawArea = sf::FloatRect(drawLeft, drawTop, drawWidth, drawHeight);

    recalculateTileCenters();
}

// --- Render ---
void Board::render( sf::RenderWindow& window, 
                    const std::array<int, 4>& playerPositions, 
                    const std::array<bool, 4>& isPlayerActive, 
                    const std::array<StatusProperty, 40>& propertyData, 
                    int animatingPlayerIdx, 
                    sf::Vector2f animatingPlayerPos, 
                    const std::array<PlayerDirection, 4>& playerDirs) {   

    // Board
    if (boardTextureLoaded) {
        sf::Sprite boardSprite(boardTexture);
        boardSprite.setPosition(boardDrawArea.left, boardDrawArea.top);
        
        float scaleX = boardDrawArea.width / boardTexture.getSize().x;
        float scaleY = boardDrawArea.height / boardTexture.getSize().y;
        boardSprite.setScale(scaleX, scaleY);
        
        window.draw(boardSprite);
    }

    // Rumah, Hotel
    for (int i = 0; i < kTotalTiles; ++i) {
        int pOwner = propertyData[i].getOwnerIndex();
        int pLevel = propertyData[i].getLevel();

        if (pOwner != -1 && pOwner < 4 && pLevel > 0) {
            std::string color = playerColorNames[pOwner];
            std::string key = (pLevel == 5) ? color + "Hotel" : color + std::to_string(pLevel);
            
            if (buildingTextures.count(key)) {
                sf::Sprite bSprite(buildingTextures[key]);
                bSprite.setScale(0.35f, 0.35f); 
                bSprite.setOrigin(bSprite.getLocalBounds().width / 2.f, bSprite.getLocalBounds().height / 2.f);
                bSprite.setPosition(getBuildingPosition(i));
                window.draw(bSprite);
            }
        }
    }

    // Karakter
    for (int i = 0; i < 4; ++i) {
        if (!isPlayerActive[i]) continue;

        sf::Vector2f center;
        if (i == animatingPlayerIdx) {
            center = animatingPlayerPos; 
        } else {
            int tileIndex = playerPositions[i] % 40;
            center = getTokenPosition(tileIndex);
            center += computeTokenOffsetForStackIndex(i);
        }

        if (isPlayerTexLoaded[i]) {
            PlayerDirection currentDir = playerDirs[i];
            sf::Sprite pSprite(playerTextures[i][currentDir]);
            
            float texW = pSprite.getTexture()->getSize().x;
            float texH = pSprite.getTexture()->getSize().y;
            float scale = 30.f / std::max(texW, texH);
            
            pSprite.setScale(scale, scale);
            pSprite.setOrigin(texW / 2.f, texH / 2.f); 
            pSprite.setPosition(center);
            window.draw(pSprite);
        } else {
            sf::CircleShape circle(15.f);
            circle.setOrigin(15.f, 15.f);
            circle.setPosition(center);
            circle.setFillColor(sf::Color(50 * i, 100, 255));
            window.draw(circle);
        }
    }

    // Debug
    if (showDebugIndices) {
        for (int i = 0; i < 40; ++i) {
            sf::Text indexText;
            indexText.setFont(debugFont);
            indexText.setString(std::to_string(i));
            indexText.setCharacterSize(12); 
            
            indexText.setFillColor(sf::Color::Red);
            indexText.setOutlineColor(sf::Color::White);
            indexText.setOutlineThickness(2.f);
            indexText.setStyle(sf::Text::Bold);

            sf::FloatRect bounds = indexText.getLocalBounds();
            indexText.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
            
            indexText.setPosition(trueTileCenters[i]);
            
            window.draw(indexText);
        }
    }
}

// --- Koordinat --- 
void Board::recalculateTileCenters() {
    const sf::FloatRect board = boardDrawArea;
    const float totalUnits = 9.0f + (2.0f * kCornerRatio);

    const float unitX = board.width / totalUnits;
    const float unitY = board.height / totalUnits;

    const float cornerX = unitX * kCornerRatio;
    const float cornerY = unitY * kCornerRatio;

    auto getCoordX = [&](int col) {
        if (col == 0) return board.left + (cornerX / 2.0f);
        if (col == 10) return board.left + board.width - (cornerX / 2.0f);
        return board.left + cornerX + ((col - 1) * unitX) + (unitX / 2.0f);
    };

    auto getCoordY = [&](int row) {
        if (row == 0) return board.top + (cornerY / 2.0f);
        if (row == 10) return board.top + board.height - (cornerY / 2.0f);
        return board.top + cornerY + ((row - 1) * unitY) + (unitY / 2.0f);
    };

    auto tileCenter = [&](int gridX, int gridY) {
        return sf::Vector2f(getCoordX(gridX), getCoordY(gridY));
    };

    tileCenters.resize(kTotalTiles);
    trueTileCenters.resize(kTotalTiles);

    // Mapping 40 Tiles  
    tileCenters[0] = tileCenter(10, 10);
    for (int i = 1; i <= 9; ++i) tileCenters[i] = tileCenter(10 - i, 10);

    tileCenters[10] = tileCenter(0, 10);
    for (int i = 11; i <= 19; ++i) tileCenters[i] = tileCenter(0, 20 - i);

    tileCenters[20] = tileCenter(0, 0);
    for (int i = 21; i <= 29; ++i) tileCenters[i] = tileCenter(i - 20, 0);

    tileCenters[30] = tileCenter(10, 0);
    for (int i = 31; i <= 39; ++i) tileCenters[i] = tileCenter(10, i - 30);

    for (int i = 0; i < kTotalTiles; ++i) {
        trueTileCenters[i] = tileCenters[i];
    }

    for (int i = 0; i < kTotalTiles; ++i) {
        tileCenters[i] = applyTileInset(i, tileCenters[i]);
    }
}

sf::Vector2f Board::applyTileInset(int tileIndex, const sf::Vector2f& center) const {
    const float totalUnits = 9.0f + (2.0f * kCornerRatio);
    const float stepX = boardDrawArea.width / totalUnits;
    const float stepY = boardDrawArea.height / totalUnits;
    const float insetX = stepX * 0.02f;
    const float insetY = stepY * 0.02f;

    sf::Vector2f adjusted = center;

    if (tileIndex >= 1 && tileIndex <= 9) adjusted.y -= insetY;
    else if (tileIndex >= 11 && tileIndex <= 19) adjusted.x += insetX;
    else if (tileIndex >= 21 && tileIndex <= 29) adjusted.y += insetY;
    else if (tileIndex >= 31 && tileIndex <= 39) adjusted.x -= insetX;
    else if (tileIndex == 0) { adjusted.x -= insetX; adjusted.y -= insetY; }
    else if (tileIndex == 10) { adjusted.x += insetX; adjusted.y -= insetY; }
    else if (tileIndex == 20) { adjusted.x += insetX; adjusted.y += insetY; }
    else if (tileIndex == 30) { adjusted.x -= insetX; adjusted.y += insetY; }

    return adjusted;
}

sf::Vector2f Board::getTileCenter(int tileIndex) const {
    if (tileIndex < 0 || tileIndex >= kTotalTiles) return sf::Vector2f(0.f, 0.f);
    return tileCenters[tileIndex];
}

sf::Vector2f Board::getBuildingPosition(int tileIndex) const {
    sf::Vector2f center = tileCenters[tileIndex];
    float offset = 43.f; 

    if (tileIndex > 0 && tileIndex < 10) return center + sf::Vector2f(0.f, -offset); 
    if (tileIndex > 10 && tileIndex < 20) return center + sf::Vector2f(offset, 0.f); 
    if (tileIndex > 20 && tileIndex < 30) return center + sf::Vector2f(0.f, offset); 
    if (tileIndex > 30 && tileIndex < 40) return center + sf::Vector2f(-offset, 0.f); 
    
    return center;
}

sf::Vector2f Board::computeTokenOffsetForStackIndex(int stackIndex) const { // Supaya ga saling tumpuk menumpuk karakternya
    const float spacing = std::min(boardDrawArea.width, boardDrawArea.height) / 80.0f;
    
    static const std::array<sf::Vector2f, 8> offsets = {
        sf::Vector2f(0.f, 0.f),   
        sf::Vector2f(1.f, 0.f),   
        sf::Vector2f(0.f, 1.f),   
        sf::Vector2f(1.f, 1.f),   
        sf::Vector2f(-1.f, 0.f),  
        sf::Vector2f(0.f, -1.f),  
        sf::Vector2f(-1.f, -1.f), 
        sf::Vector2f(1.f, -1.f)
    };
    
    return sf::Vector2f(offsets[stackIndex % 8].x * spacing, offsets[stackIndex % 8].y * spacing);
}

sf::Vector2f Board::getTokenPosition(int tileIndex) const {
    if (tileIndex < 0 || tileIndex >= 40) {
        return sf::Vector2f(0.f, 0.f); 
    }
    
    return tileCenters[tileIndex];
}