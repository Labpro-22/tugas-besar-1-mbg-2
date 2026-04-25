#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <array>
#include <map>
#include <string>

struct PropertyStatus {
    int ownerIndex = -1; // -1 jika belum dimiliki, 0-3 jika sudah
    int level = 0;       // 0: kosong, 1-4: rumah, 5: hotel
};

enum class PlayerDirection {
    Forward,
    Back,
    Left,
    Right
};

class Board {
private:
    sf::Texture boardTexture;
    sf::Sprite boardSprite;
    sf::FloatRect boardDrawArea;
    std::vector<sf::Vector2f> tileCenters;
    std::vector<sf::Vector2f> trueTileCenters;
    bool boardTextureLoaded;

    void recalculateTileCenters();
    sf::Vector2f applyTileInset(int tileIndex, const sf::Vector2f& center) const;

    // --- PLAYER TEXTURES ---
    std::array<std::map<PlayerDirection, sf::Texture>, 4> playerTextures;
    std::array<bool, 4> isPlayerTexLoaded;

    // --- BUILDING TEXTURES ---
    std::map<std::string, sf::Texture> buildingTextures;
    std::array<PropertyStatus, 40> properties; 
    std::array<std::string, 4> playerColorNames = {"Brown", "Green", "Red", "Blue"};

    // --- DEBUG ---
    sf::Font debugFont;
    bool showDebugIndices;

public:
    Board();
    void loadAssets();
    void fitToLayout(sf::FloatRect containerArea);
    
    // Fungsi Render dengan Animasi
    void render(sf::RenderWindow& window, const std::array<int, 4>& playerPositions, const std::array<bool, 4>& isPlayerActive, const std::array<PropertyStatus, 40>& propertyData, int animatingPlayerIdx, sf::Vector2f animatingPlayerPos, const std::array<PlayerDirection, 4>& playerDirs);

    // Fungsi Pengambil Koordinat
    sf::Vector2f getTileCenter(int tileIndex) const;
    sf::Vector2f getBuildingPosition(int tileIndex) const;
    sf::Vector2f getTokenPosition(int tileIndex) const; 
    sf::Vector2f computeTokenOffsetForStackIndex(int stackIndex) const;
};