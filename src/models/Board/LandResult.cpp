#include "LandResult.hpp"

LandResult::LandResult(LandEventType type, Tile* sourceTile, Tile* targetTile, Player* sourcePlayer, Player* targetPlayer, int amount, bool requiresChoice)
    : type(type), sourceTile(sourceTile), targetTile(targetTile), sourcePlayer(sourcePlayer), targetPlayer(targetPlayer), amount(amount), requiresChoice(requiresChoice) {}

LandEventType LandResult::getType() const {
    return type;
}

Tile* LandResult::getSourceTile() const {
    return sourceTile;
}

Tile* LandResult::getTargetTile() const {
    return targetTile;
}

Player* LandResult::getSourcePlayer() const {
    return sourcePlayer;
}

Player* LandResult::getTargetPlayer() const {
    return targetPlayer;
}

int LandResult::getAmount() const {
    return amount;
}

bool LandResult::isRequiresChoice() const {
    return requiresChoice;
}
