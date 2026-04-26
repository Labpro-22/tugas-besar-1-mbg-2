#pragma once
#include "GameContext.hpp"
#include "Player.hpp"
#include "Tile.hpp"
#include "InputHandler.hpp"

class GameView {
public:
    virtual ~GameView() = default;

    virtual void updateBoardState(GameContext& G) = 0;
    virtual void showDiceAnimation(int dice1, int dice2) = 0;
    virtual void triggerPopup(const string& popupType, Tile* tileData, const std::string& popupMessage = "") = 0;
    virtual void closeUI() = 0;

    virtual bool isAnimationPlaying() const = 0;
    virtual int getPopupResponse() = 0;
    virtual int getLastPopupTileIndex() const = 0;
    virtual CommandType getGUICommand() = 0;
    virtual std::string getStringInput(const std::string& prompt) = 0;
    virtual int getAuctionBid(int minBid, int activePlayerIdx, const std::string& propName, int currentMaxBid, int highBidderIdx) = 0;
    virtual int getSkillCardChoice(const std::vector<SkillCard*>& cards, bool isDiscardMode) = 0;
    virtual int getIntChoiceFromList(const std::string& prompt, const std::vector<std::string>& options) = 0;
    virtual void showInfoPopup(const std::string& title, const std::string& message) = 0;
    virtual void addLog(const std::string& msg) = 0;
};