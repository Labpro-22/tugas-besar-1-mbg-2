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
    virtual void triggerPopup(const string& popupType, Tile* tileData) = 0;
    virtual void closeUI() = 0;

    virtual bool isAnimationPlaying() const = 0;
    virtual int getPopupResponse() = 0;

    virtual CommandType getGUICommand() = 0;
};