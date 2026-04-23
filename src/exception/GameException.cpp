#include "GameException.hpp"

GameException::GameException(const string& message) : message(message){}

const char* GameException::what() const noexcept{
    return message.c_str();
}

// ECONOMY
InsufficientFundsException::InsufficientFundsException(const int& required, const int& cur)
    : GameException("Uang tidak mencukup! Butuh: M" + to_string(required) + ", Anda hanya memiliki M" + to_string(cur)),required(required),cur(cur) {}

// CARD
InventoryFullException::InventoryFullException():GameException("Inventory kamu penuh! Anda sudah memiliki 3 kartu."){}

CardNotFoundException::CardNotFoundException(const string& cardName): GameException("Kamu tidak memiliki kartu: " + cardName + "!"),cardName(cardName){}

AlreadyUseCardException::AlreadyUseCardException():GameException("Kamu hanya boleh menggunakan satu kartu dalam satu giliran!"){}

UseCardAfterDiceException::UseCardAfterDiceException():GameException("Kamu tidak bisa menggunakan kartu setelah mengocok dadu!"){}

// Property
TileNotFoundException::TileNotFoundException(const string& code): GameException("Petak dengan kode " + code + " tidak ditemukan!"), code(code){}

PropertyNotOwnedException::PropertyNotOwnedException(): GameException("Properti ini belum dimiliki oleh siapapun!"){}

PropertyAlreadyOwnedException::PropertyAlreadyOwnedException(): GameException("Properti ini sudah dimiliki oleh orang lain!"){}

PropertyMortgagedException::PropertyMortgagedException(): GameException("Properti ini sedang digadaikan!"){}

NotMortgagedException::NotMortgagedException(): GameException("Properti ini tidak sedang digadaikan!"){}

HasBuildingForMortgage::HasBuildingForMortgage(): GameException("Masih ada bangunan di color group ini. Bangunan harus dijual terlebih dahulu!"){}

UnevenBuildingException::UnevenBuildingException(const string& color): GameException("Pembangunan harus merata! Selisih jumlah rumah di color group " + color + " tidak boleh lebih dari satu!"){}

MaxBuildingException::MaxBuildingException():GameException("Petak ini sudah mencapai batas pembangunan!"){}

UpgradeHotelException::UpgradeHotelException():GameException("Seluruh petak dalam color group harus memiliki 4 rumah sebelum bisa upgrade ke Hotel!"){}

// INPUT 
InvalidCommandException::InvalidCommandException():GameException("Perintah tidak valid!"){}

InvalidArgumentException::InvalidArgumentException(const string& arg):GameException("Argumen: " + arg + " tidak valid!"), arg(arg){}

// DICE
RollDiceException::RollDiceException(): GameException("Angka dadu harus berada di antara 1 sampai 6!"){}

// SYSTEM 
FileNotFoundException::FileNotFoundException(const string& filename): GameException("File: " + filename + " tidak ditemukan!"){}
