#include "gui/popups/akta/PropertyAktaPopup.hpp"

PropertyAktaPopup::PropertyAktaPopup(const PropertyAktaData& propertyData) : data(propertyData) {
    // Ukuran kartu (Portrait)
    cardRect = sf::FloatRect(350.f, 80.f, 320.f, 480.f);
    
    closeBtnRect = sf::FloatRect(cardRect.left + 110.f, cardRect.top + cardRect.height + 15.f, 100.f, 40.f);
    popupRect = cardRect; 
}

void PropertyAktaPopup::drawRightAlignedText(sf::RenderWindow& window, std::string textStr, float rightX, float y, int size, sf::Color color, bool isBold) {
    sf::Text text = createText(textStr, 0, 0, size, color); // Koordinat sementara 0,0
    if (isBold) text.setStyle(sf::Text::Bold);
    
    float textWidth = text.getLocalBounds().width;
    text.setPosition(rightX - textWidth, y);
    window.draw(text);
}

void PropertyAktaPopup::render(sf::RenderWindow& window) {
    sf::RectangleShape cardBg(sf::Vector2f(cardRect.width, cardRect.height));
    cardBg.setPosition(cardRect.left, cardRect.top);
    cardBg.setFillColor(sf::Color::White);
    cardBg.setOutlineThickness(3.f);
    cardBg.setOutlineColor(sf::Color::Black);
    window.draw(cardBg);

    sf::FloatRect headerRect(cardRect.left + 10.f, cardRect.top + 10.f, cardRect.width - 20.f, 70.f);
    sf::RectangleShape header(sf::Vector2f(headerRect.width, headerRect.height));
    header.setPosition(headerRect.left, headerRect.top);
    header.setFillColor(data.colorCode);
    header.setOutlineThickness(2.f);
    header.setOutlineColor(sf::Color::Black);
    window.draw(header);

    window.draw(createText("TITLE DEED", headerRect.left + (headerRect.width/2.f) - 35.f, headerRect.top + 10.f, 12, sf::Color::White));
    
    sf::Text nameText = createText(data.name, 0, 0, 20, sf::Color::White);
    nameText.setStyle(sf::Text::Bold);
    nameText.setOrigin(nameText.getLocalBounds().width / 2.f, 0.f);
    nameText.setPosition(headerRect.left + (headerRect.width/2.f), headerRect.top + 30.f);
    window.draw(nameText);

    float startY = cardRect.top + 100.f;
    float leftX = cardRect.left + 20.f;
    float rightX = cardRect.left + cardRect.width - 20.f;
    float spacing = 25.f;

    // Base Rent
    window.draw(createText("Rent", leftX, startY, 16, sf::Color::Black));
    drawRightAlignedText(window, "M" + std::to_string(data.baseRent), rightX, startY, 16, sf::Color::Black);
    
    // Rent with Houses
    for (size_t i = 0; i < data.rentWithHouses.size(); ++i) {
        startY += spacing;
        window.draw(createText("Rent with " + std::to_string(i+1) + " House", leftX, startY, 16, sf::Color::Black));
        drawRightAlignedText(window, "M" + std::to_string(data.rentWithHouses[i]), rightX, startY, 16, sf::Color::Black);
    }

    // Rent with Hotel
    startY += spacing;
    window.draw(createText("Rent with HOTEL", leftX, startY, 16, sf::Color::Black));
    drawRightAlignedText(window, "M" + std::to_string(data.rentWithHotel), rightX, startY, 16, sf::Color::Black);

    startY += 30.f;
    sf::RectangleShape line(sf::Vector2f(cardRect.width - 40.f, 2.f));
    line.setPosition(leftX, startY);
    line.setFillColor(sf::Color::Black);
    window.draw(line);

    // Bangunan & Gadai
    startY += 15.f;
    window.draw(createText("Mortgage Value", leftX, startY, 14, sf::Color::Black));
    drawRightAlignedText(window, "M" + std::to_string(data.mortgageValue), rightX, startY, 14, sf::Color::Black);

    startY += spacing;
    window.draw(createText("Houses cost", leftX, startY, 14, sf::Color::Black));
    drawRightAlignedText(window, "M" + std::to_string(data.houseCost) + " each", rightX, startY, 14, sf::Color::Black);

    startY += spacing;
    window.draw(createText("Hotels,, plus 4 houses", leftX, startY, 14, sf::Color::Black));
    drawRightAlignedText(window, "M" + std::to_string(data.hotelCost) + " each", rightX, startY, 14, sf::Color::Black);

    // Status Kepemilikan 
    startY += 40.f;
    std::string statusStr = data.ownerName.empty() ? "Status: UNOWNED" : "Status: OWNED (" + data.ownerName + ")";
    sf::Color statusColor = data.ownerName.empty() ? sf::Color(100, 100, 100) : sf::Color::Red;
    window.draw(createText(statusStr, leftX, startY, 16, statusColor));

    draw3DPanel(window, closeBtnRect, sf::Color(200, 200, 200), false);
    window.draw(createText("Tutup", closeBtnRect.left + 25.f, closeBtnRect.top + 8.f, 18, sf::Color::Black));
}

PopupResult PropertyAktaPopup::handleMouseClick(float mouseX, float mouseY) {
    if (containsPoint(closeBtnRect, mouseX, mouseY)) {
        return PopupResult::CLOSE_POPUP;
    }
    return PopupResult::NONE;
}