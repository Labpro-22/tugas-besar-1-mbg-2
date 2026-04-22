#pragma once
#include "GameContext.hpp"
#include "PropertyTile.hpp"
#include "StreetTile.hpp"
#include "RailroadTile.hpp"
#include "UtilityTile.hpp"

class DisplayView
{
public:
    // Rendering board 
    void renderBoard(GameContext G);

    // Rendering Akta
    void renderAkta(GameContext G, string code);

    // Rendering property owned by current player
    void renderProperty(GameContext G);

    // Rendering game menu
    void showMenu(GameContext G);
    
    // for buying unowned properties
    // buying StreetTile specific rendering
    void renderBuyStreet(GameContext G, StreetTile* tile);
    void renderBoughtStreet(GameContext G, StreetTile* tile);
    
    // getting railroad and utility specific rendering
    void renderGetRailroad(GameContext G, RailroadTile* tile);
    void renderGetUtility(GameContext G, UtilityTile* tile);
    
    void renderDiceRoll(GameContext G);
    void renderDiceControl(GameContext G);
    void renderRent(GameContext G, PropertyTile* tile);
    void renderMortgage(GameContext G, PropertyTile* tile);

private:
    // Helper function to fit text to a specific width
    static const int CARD_INNER_WIDTH = 30;
    static const int BOARD_CELL_WIDTH = 10;
    
    string fitToWidth(const string& text, int width) const;
    void printCardLine(const string& content) const;
    
    // Rendering landed on tile
    void renderTile(GameContext G);

    int getBoardSideLength(GameContext G) const;
    int getBoardCenterWidth(int sideLength) const;
    string normalizeTileColorCode(Tile* tile) const;
    string getAnsiColorByCode(const string& colorCode) const;
    string colorizeText(const string& text, const string& colorCode) const;
    
    // --- Board rendering helpers ---
    string formatCellContent(const string& content) const;
    string formatCenterContent(const string& content, int centerWidth) const;
    vector<string> buildCenterLines(GameContext G, int centerWidth, int sideRows) const;
    string makeBorderLine(int sideLength) const;
    string makeSideBorderLine(int centerWidth) const;
    
    string getTileLine1(Tile* tile) const;
    string getTileLine2(int tileIdx, Tile* tile, GameContext G) const;
    
    void printTopRow(GameContext G, int sideLength);
    void printBottomRow(GameContext G, int sideLength);
    void printSideRows(GameContext G, int sideLength, int centerWidth);
    
    vector<Player*> getPlayersOnTile(int tileIdx, GameContext G) const;
    string buildOwnershipInfo(Tile* tile, GameContext G) const;
    string buildPlayerPositionInfo(int tileIdx, GameContext G) const;

    void renderOneProperty(PropertyTile* tile);
};
