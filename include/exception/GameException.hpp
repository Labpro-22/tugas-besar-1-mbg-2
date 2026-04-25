#pragma once
#include <exception>
#include <string>

using namespace std;

class GameException : public exception{
protected : 
    string message; 
public : 
    GameException(const string& message); 
    virtual const char* what() const noexcept;
};


// ECONOMY
class InsufficientFundsException : public GameException { 
private : 
    int required;
    int cur; 
public : 
    InsufficientFundsException(const int& required, const int& cur);
    int getRequired() const;
    int getCurrentBalance() const;
};

class AuctionTriggerException : public GameException{ 
public:
    AuctionTriggerException();
};

class BankruptcyException : public InsufficientFundsException{
public:
    BankruptcyException(const string& message, const int& required, const int& cur);
};

// CARD 
class InventoryFullException : public GameException{
public : 
    InventoryFullException();
};

class CardNotFoundException : public GameException{
private : 
    string cardName; 
public : 
   CardNotFoundException(const string& cardName);
};

class AlreadyUseCardException : public GameException{ 
public : 
    AlreadyUseCardException();
};

class UseCardAfterDiceException : public GameException{ 
public : 
    UseCardAfterDiceException();
};

// Property
class TileNotFoundException : public GameException{
private : 
    string code; 
public : 
    TileNotFoundException(const string& code);
};

class PropertyNotOwnedException : public GameException{
public : 
    PropertyNotOwnedException();
};

class PropertyAlreadyOwnedException : public GameException{
public : 
    PropertyAlreadyOwnedException();
};

// MORTGAGE
class PropertyMortgagedException : public GameException{
public : 
    PropertyMortgagedException();
};

class NotMortgagedException : public GameException{
public : 
    NotMortgagedException();
};

// BUILD 
class UnevenBuildingException : public GameException{
private : 
    string color;     
public : 
    UnevenBuildingException(const string& color);
};

class MaxBuildingException : public GameException{
public : 
    MaxBuildingException();
};

class UpgradeHotelException : public GameException{
public : 
    UpgradeHotelException();
};

// INPUT 
class InvalidCommandException : public GameException{
public : 
    InvalidCommandException();
};

class InvalidArgumentException : public GameException{
private : 
    string arg;
public : 
    InvalidArgumentException(const string& arg);
};

// Player
class DuplicateNameException : public GameException{
private : 
    string name; 
    DuplicateNameException(const string& name);
};

// Dice
class RollDiceException : public GameException{
public : 
    RollDiceException();
};

// System 
class FileNotFoundException : public GameException{
private : 
    string filename; 
public : 
    FileNotFoundException(const string& filename);
};

class SaveGameException : public GameException{
private :
    string filename;
public : 
    SaveGameException(const string& filename);
};

class LoadGameException : public GameException{
private :
    string filename;
public : 
    LoadGameException(const string& filename);
};
