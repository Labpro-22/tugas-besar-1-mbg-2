<<<<<<< HEAD
#pragma once
#include "PropertyTile.hpp"
#include "GameContext.hpp"
#include "Player.hpp"
#include "ActionCard.hpp"
#include "SkillCard.hpp"
#include <map>
#include "CardDeck.hpp"

using namespace std;

class EffectController
{
public:
    void applyFestival(PropertyTile* tile);
    void decrementDurations();
    
    void execute(ActionCard& card, Player& currentPlayer, GameContext& ctx);
    void execute(SkillCard& card, Player& currentPlayer, GameContext& ctx, InputHandler& input);
};

=======
#pragma once
#include "PropertyTile.hpp"
#include "Card.hpp"
#include "Player.hpp"
#include <map>
#include "CardDeck.hpp"

using namespace std;

class EffectController
{
public:
    void applyFestival(PropertyTile* tile);
    void decrementDurations();
    void executeCard(Player* player, CardType card);
};

>>>>>>> 2242d01afd5f4da5b17612f349db6105653b5b58
