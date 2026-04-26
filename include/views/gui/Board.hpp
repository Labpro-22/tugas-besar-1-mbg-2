#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <array>
#include <map>
#include <string>

class StatusProperty{
    private:
        int ownerIndex; //-1 jika belum dimiliki, 0-3 jika sudah
        int level; //0 = kosong, 1-4 = rumah, 5 = hotel
    public:
        StatusProperty() : ownerIndex(-1), level(0) {}
        StatusProperty(int owner, int lvl) : ownerIndex(owner), level(lvl) {}

        int getOwnerIndex() const { return ownerIndex; }
        void setOwnerIndex(int owner) { ownerIndex = owner; }

        int getLevel() const { return level; }
        void setLevel(int lvl) { level = lvl; }
};

enum class PlayerDirection {
    Forward,
    Back,
    Left,
    Right
}; 

class Board {
    private:
        // --- Board ---
        sf::FloatRect boardDrawArea;
        std::vector<sf::Vector2f> tileCenters;
        std::vector<sf::Vector2f> trueTileCenters;

        // --- Texture, Font
        sf::Texture boardTexture;
        bool boardTextureLoaded;

        std::array<std::map<PlayerDirection, sf::Texture>, 4> playerTextures;
        std::array<bool, 4> isPlayerTexLoaded;

        std::map<std::string, sf::Texture> buildingTextures;
        sf::Font debugFont;

        // --- Debug ---
        std::array<StatusProperty, 40> properties; 
        std::array<std::string, 4> playerColorNames = {"Brown", "Green", "Red", "Blue"};
        bool showDebugIndices;

        // --- Helper ---
        void recalculateTileCenters();
        sf::Vector2f applyTileInset(int tileIndex, const sf::Vector2f& center) const;
    
    public:
        Board();
        void loadAssets();
        void fitToLayout(sf::FloatRect containerArea);
        
        // --- Render ---
        void render(sf::RenderWindow& window, 
                    const std::array<int, 4>& playerPositions, 
                    const std::array<bool, 4>& isPlayerActive, 
                    const std::array<StatusProperty, 40>& propertyData, 
                    int animatingPlayerIdx, 
                    sf::Vector2f animatingPlayerPos, 
                    const std::array<PlayerDirection, 4>& playerDirs);

        // --- Getter --- 
        sf::Vector2f getTileCenter(int tileIndex) const;
        sf::Vector2f getBuildingPosition(int tileIndex) const;
        sf::Vector2f getTokenPosition(int tileIndex) const; 
        sf::Vector2f computeTokenOffsetForStackIndex(int stackIndex) const;        
};