#pragma once
#include "PropertyTile.hpp"
#include "GameContext.hpp"
#include "Player.hpp"
#include "ActionCard.hpp"
#include "SkillCard.hpp"
#include "InputHandler.hpp"
#include "DisplayView.hpp"
#include <map>
#include "CardDeck.hpp"

using namespace std;

class EffectController
{
public:
    void  handleFestival(Tile* tile);
    void decrementDurations(GameContext* context);

    void execute(ActionCard& card, Player& currentPlayer, GameContext& ctx);
    void execute(SkillCard& card, Player& currentPlayer, GameContext& ctx, InputHandler& input, DisplayView& display);
};

