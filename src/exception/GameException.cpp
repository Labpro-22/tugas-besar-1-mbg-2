#include "GameException.hpp"
GameException::GameException(const string& message) : message(message){}

const char* GameException::what() const noexcept{
    return message.c_str();
}

// ECONOMY
InsufficientFundsException::InsufficientFundsException(const int& required, const int& cur)
    : GameException("Insufficient funds! Required: M" + to_string(required) + ", you only have M" + to_string(cur)),required(required),cur(cur) {}

int InsufficientFundsException::getRequired() const {
    return required;
}

int InsufficientFundsException::getCurrentBalance() const {
    return cur;
}

AuctionTriggerException::AuctionTriggerException(): GameException("You don't have enough funds to purchase this property! Starting auction..."){}

BankruptcyException::BankruptcyException(const string& message, const int& required, const int& cur, Player* creditor, Tile* bankruptTile): InsufficientFundsException(required, cur), creditor(creditor), bankruptTile(bankruptTile) {}

Player* BankruptcyException::getCreditor() const {
    return creditor;
}

Tile* BankruptcyException::getBankruptTile() const {
    return bankruptTile;
}

// CARD
InventoryFullException::InventoryFullException():GameException("Your inventory is full! You already have 3 cards."){}

CardNotFoundException::CardNotFoundException(const string& cardName): GameException("You do not own the card: " + cardName + "!"),cardName(cardName){}

AlreadyUseCardException::AlreadyUseCardException():GameException("You can only use one card per turn!"){}

UseCardAfterDiceException::UseCardAfterDiceException():GameException("You can't use a card after rolling the dice!"){}

// Property
TileNotFoundException::TileNotFoundException(const string& code): GameException("Tile with code " + code + " not found!"), code(code){}

PropertyNotOwnedException::PropertyNotOwnedException(): GameException("Property isn't owned by anyone!"){}

PropertyAlreadyOwnedException::PropertyAlreadyOwnedException(): GameException("Property is already owned by another player!"){}

PropertyMortgagedException::PropertyMortgagedException(): GameException("Property is currently mortgaged!"){}

NotMortgagedException::NotMortgagedException(): GameException("This property is not currently mortgaged!"){}

UnevenBuildingException::UnevenBuildingException(const string& color): GameException("Buildings need to be even! The difference in the number of houses in color group " + color + " can't be more than one!"){}

MaxBuildingException::MaxBuildingException():GameException("Tile has reached maximum building limit!"){}

UpgradeHotelException::UpgradeHotelException():GameException("All tiles in color group must have 4 houses before upgrading to a hotel!"){}

// INPUT 
InvalidCommandException::InvalidCommandException():GameException("Invalid command!"){}

InvalidArgumentException::InvalidArgumentException(const string& arg):GameException("Argument: " + arg + " is invalid!"), arg(arg){}

// Player
DuplicateNameException::DuplicateNameException(const string& name):GameException("Name " + name + " has already been used! Please use another name!"){}

// DICE
RollDiceException::RollDiceException(): GameException("Dice number must be between 1 to 6!"){}

// SYSTEM 
FileNotFoundException::FileNotFoundException(const string& filename): GameException("File: " + filename + " not found!"){}

SaveGameException::SaveGameException(const string& filename): GameException("Failed to save to " + filename){}

LoadGameException::LoadGameException(const string& filename): GameException("Failed to load game from " + filename){}