#pragma once
#include "GameContext.hpp"

// Tiles
#include "PropertyTile.hpp"
#include "StreetTile.hpp"
#include "RailroadTile.hpp"
#include "UtilityTile.hpp"
#include "TaxTile.hpp"
#include "FestivalTile.hpp"
#include "CardTile.hpp"

#include "GameLogger.hpp"
class DisplayView
{
public:
    // Rendering board 
    void renderBoard(GameContext G);
    
    // Dice Display
    void renderDiceRoll(GameContext G);
    void renderDiceControl(GameContext G);\

    // Rendering Akta
    void renderAkta(GameContext G, string code);
    
    // Rendering property owned by current player
    void renderProperty(GameContext G);
    
    // for buying unowned properties
    // buying StreetTile specific rendering
    void renderBuyStreet(GameContext G, StreetTile* tile);
    void renderBoughtStreet(GameContext G, StreetTile* tile);
    
    // getting railroad and utility specific rendering
    void renderGetRailroad(GameContext G, RailroadTile* tile);
    void renderGetUtility(GameContext G, UtilityTile* tile);

    // Rendering rent, mortgage, and failed payment
    void renderRent(GameContext G, PropertyTile* tile);
    void renderMortgage(GameContext G, PropertyTile* tile);
    void renderCantPay(GameContext G, int amountOwed);

    // rendering tax tile
    void renderTax(GameContext G, TaxTile *tile);
    void renderPayTax(GameContext G, int choose);

    // Mortgage
    void renderMortgageStart(GameContext G, vector<PropertyTile*> unmortgagedTiles);
    void renderMortgageResult(GameContext G, PropertyTile* tile);
    void renderMortgageGroupColorStart(GameContext G, vector<StreetTile*> sameColorProps);
    void renderMortgageGroupColorResult(GameContext G, string choice, vector<StreetTile*> sameColorProps);
    
    // Redeem Mortgage
    void renderRedeemStart(GameContext G, vector<PropertyTile*> mortgagedTiles);
    void renderRedeemChoose(GameContext G, vector<PropertyTile*> mortgagedTiles, int choose);
    
    // Build
    void renderBuildStart(GameContext G, map<string, vector<StreetTile*>> tiles);
    void renderBuildResult(GameContext G, StreetTile* tile, string choice);
    
    // Auction 
    void renderAuctionStart(GameContext G, PropertyTile* tile);
    void renderAuctionLine(string playerName);
    void HighestBidder(string playerName, int bidAmount);
    void renderAuctionResult(string winnerName, int winningBid);
    
    // Rendering game menu
    void showMenu(GameContext G);

    // Festival
    void renderFestivalTile(GameContext G, FestivalTile *tile);
    void renderFestivalResult(GameContext G, StreetTile* tile);
    void InputUnvalidFestivalProperty(GameContext G);

    //SaveLoad
    void renderSaveSuccess(GameContext G, string filename);
    void renderSaveFailure(GameContext G, string filename);
    void renderSaveFilenameUsed(GameContext G, string filnename);
    void renderOverwrite(GameContext G, string filename, string choice);
    void renderLoadSuccess(GameContext G, string filename);
    void renderLoadFailure(GameContext G, string filename, bool fileExists);
    
    //Finish Game
    void renderGameOverMaxTurn(GameContext G);

    // Card Tile
    void renderCardTile(GameContext G, CardTile* tile, Card );

    // Use Card
    void renderUseCardMenu(GameContext G, Card* card);
    
    // Player Info
    void renderPlayerInfo(GameContext G, Player* player);
    void renderPlayer(GameContext G);
    
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

};
