#pragma once
#include "gui/popups/Popup.hpp"
#include <string>
#include <vector>

struct PropertyAktaData {
    std::string name;
    std::string colorGroupName;
    sf::Color colorCode;
    
    int price;
    int mortgageValue;
    int baseRent;
    std::vector<int> rentWithHouses; // Index 0-3 untuk 1-4 rumah
    int rentWithHotel;
    
    int houseCost;
    int hotelCost;
    
    std::string ownerName; // Kosong jika belum dimiliki
};

class PropertyAktaPopup : public Popup {
private:
    PropertyAktaData data;
    sf::FloatRect cardRect;
    sf::FloatRect closeBtnRect;

    // Helper untuk menggambar teks rata kanan (untuk harga)
    void drawRightAlignedText(sf::RenderWindow& window, std::string text, float rightX, float y, int size, sf::Color color, bool isBold = false);

public:
    PropertyAktaPopup(const PropertyAktaData& propertyData);
    
    void render(sf::RenderWindow& window) override;
    PopupResult handleMouseClick(float mouseX, float mouseY) override;
};