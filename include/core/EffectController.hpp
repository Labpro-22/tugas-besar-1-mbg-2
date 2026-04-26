#pragma once
#include "PropertyTile.hpp"
#include "GameContext.hpp"
#include "Player.hpp"
#include "ActionCard.hpp"
#include "SkillCard.hpp"
#include "InputHandler.hpp"
#include "DisplayView.hpp"
#include "EconomyController.hpp"
#include "BankruptcyController.hpp"
#include "GameLogger.hpp"
#include <map>
#include "CardDeck.hpp"
#include <vector>

class GameView;

using namespace std;

class EffectController
{
public:
    void handleFestival( GameContext* gameContext, DisplayView* display, InputHandler* inputHandler, GameLogger& logger );
    void decrementDurations(GameContext* context);

    void execute(ActionCard& card, Player& currentPlayer, GameContext& ctx, BankruptcyController& bank, InputHandler& inputHandler, DisplayView& display, EconomyController& eco, GameLogger& logger);
    void execute(SkillCard& card, Player& currentPlayer, GameContext& ctx, InputHandler& input, DisplayView& display, GameLogger& logger, bool isGUIMode = false, GameView* guiView = nullptr);

    //helper
    bool codeInOwned( string code, vector<StreetTile*> streetTile );
    bool codeInBoard( string code, GameContext* g );
};

