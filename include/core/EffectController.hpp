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
#include <map>
#include "CardDeck.hpp"
#include <vector>

using namespace std;

class EffectController
{
public:
    void handleFestival( GameContext* gameContext, DisplayView* display, InputHandler* inputHandler );
    void decrementDurations(GameContext* context);

    void execute(ActionCard& card, Player& currentPlayer, GameContext& ctx, BankruptcyController& bank, InputHandler& inputHandler, DisplayView& display, EconomyController& eco);
    void execute(SkillCard& card, Player& currentPlayer, GameContext& ctx, InputHandler& input, DisplayView& display);

    //helper
    bool codeInOwned( string code, vector<StreetTile*> streetTile );
    bool codeInBoard( string code, GameContext* g );
};

