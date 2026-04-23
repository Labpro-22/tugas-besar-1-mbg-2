#include "DisplayView.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <algorithm>
using namespace std;

// ============ EXISTING HELPERS (CARD RENDERING) ============
string DisplayView::fitToWidth(const string& text, int width) const {
    if ((int)text.size() > width) {
        if (width <= 3) {
            return text.substr(0, width);
        }
        return text.substr(0, width - 3) + "...";
    }
    return text;
}

void DisplayView::printCardLine(const string& content) const {
    string text = fitToWidth(content, CARD_INNER_WIDTH);
    int padding = CARD_INNER_WIDTH - (int)text.size();
    if (padding < 0) {
        padding = 0;
    }
    cout << "|" << text << string(padding, ' ') << "|" << endl;
}

int DisplayView::getBoardSideLength(GameContext G) const {
    int totalTiles = G.getBoard().getTotalTile();
    if (totalTiles < 8 || totalTiles % 4 != 0) {
        return -1;
    }
    return (totalTiles / 4) + 1;
}

int DisplayView::getBoardCenterWidth(int sideLength) const {
    int totalWidth = sideLength * BOARD_CELL_WIDTH + (sideLength + 1);
    int centerWidth = totalWidth - (2 * BOARD_CELL_WIDTH + 4);
    if (centerWidth < 10) {
        centerWidth = 10;
    }
    return centerWidth;
}

string DisplayView::normalizeTileColorCode(Tile* tile) const {
    if (tile == nullptr) {
        return "DF";
    }

    string color = tile->getColor();
    if ((int)color.size() > 2) {
        color = color.substr(0, 2);
    }
    if (color.empty() || color == "DE") {
        color = "DF";
    }
    transform(color.begin(), color.end(), color.begin(), ::toupper);
    return color;
}

string DisplayView::getAnsiColorByCode(const string& colorCode) const {
    if (colorCode == "CK") return "\033[38;5;94m";   // coklat
    if (colorCode == "MR") return "\033[31m";        // merah
    if (colorCode == "BM") return "\033[96m";        // biru muda
    if (colorCode == "KN") return "\033[33m";        // kuning
    if (colorCode == "PK") return "\033[95m";        // pink
    if (colorCode == "HJ") return "\033[32m";        // hijau
    if (colorCode == "OR") return "\033[38;5;208m";  // oranye
    if (colorCode == "BT") return "\033[34m";        // biru tua
    if (colorCode == "AB") return "\033[36m";        // utilitas (aqua)
    return "\033[37m";                                // default/aksi
}

string DisplayView::colorizeText(const string& text, const string& colorCode) const {
    const string reset = "\033[0m";
    return getAnsiColorByCode(colorCode) + text + reset;
}

// ============ BOARD RENDERING HELPERS ============

// Format cell content ke lebar tetap dengan padding
string DisplayView::formatCellContent(const string& content) const {
    string fitted = fitToWidth(content, BOARD_CELL_WIDTH);
    int padding = BOARD_CELL_WIDTH - (int)fitted.size();
    if (padding < 0) padding = 0;
    return fitted + string(padding, ' ');
}

string DisplayView::formatCenterContent(const string& content, int centerWidth) const {
    string fitted = fitToWidth(content, centerWidth);
    int totalPadding = centerWidth - (int)fitted.size();
    if (totalPadding < 0) {
        totalPadding = 0;
    }

    int leftPadding = totalPadding / 2;
    int rightPadding = totalPadding - leftPadding;
    return string(leftPadding, ' ') + fitted + string(rightPadding, ' ');
}

vector<string> DisplayView::buildCenterLines(GameContext G, int centerWidth, int sideRows) const {
    vector<string> rawLines;
    rawLines.push_back("==================================");
    rawLines.push_back("||          NIMONSPOLI          ||");
    rawLines.push_back("");
    rawLines.push_back("TURN " + to_string(G.getCurrentTurn()) + " / " + to_string(G.getMaxTurns()));
    rawLines.push_back("----------------------------------");
    rawLines.push_back("LEGENDA KEPEMILIKAN & STATUS");
    rawLines.push_back("P1-P4 : Properti milik Pemain 1-4");
    rawLines.push_back("^     : Rumah Level 1");
    rawLines.push_back("^^    : Rumah Level 2");
    rawLines.push_back("^^^   : Rumah Level 3");
    rawLines.push_back("*     : Hotel (Maksimal)");
    rawLines.push_back("(1)-(4): Bidak (IN=Tahanan, V=Mampir)");
    rawLines.push_back("----------------------------------");
    rawLines.push_back("KODE WARNA:");
    rawLines.push_back("[CK]=Coklat    [MR]=Merah");
    rawLines.push_back("[BM]=Biru Muda [KN]=Kuning");
    rawLines.push_back("[PK]=Pink      [HJ]=Hijau");
    rawLines.push_back("[OR]=Orange    [BT]=Biru Tua");

    int requiredLines = sideRows * 2;
    vector<string> centerLines;
    centerLines.reserve(requiredLines);

    for (int i = 0; i < requiredLines; ++i) {
        if (i < (int)rawLines.size()) {
            centerLines.push_back(formatCenterContent(rawLines[i], centerWidth));
        } else {
            centerLines.push_back(string(centerWidth, ' '));
        }
    }
    return centerLines;
}

// Buat garis border atas/bawah papan
string DisplayView::makeBorderLine(int sideLength) const {
    string border;
    for (int i = 0; i < sideLength; ++i) {
        border += "+" + string(BOARD_CELL_WIDTH, '-');
    }
    border += "+";
    return border;
}

// Buat garis border samping (untuk sisi kiri dan kanan)
string DisplayView::makeSideBorderLine(int centerWidth) const {
    return "+" + string(BOARD_CELL_WIDTH, '-') + 
           "+" + string(centerWidth, ' ') + 
           "+" + string(BOARD_CELL_WIDTH, '-') + "+";
}

// Ambil semua pemain yang sedang di tile tertentu
vector<Player*> DisplayView::getPlayersOnTile(int tileIdx, GameContext G) const {
    vector<Player*> result;
    for (Player& p : G.getPlayers()) {
        if (p.getPosition() == tileIdx) {
            result.push_back(&p);
        }
    }
    return result;
}

// Build info kepemilikan: P1, rumah ^, hotel *
string DisplayView::buildOwnershipInfo(Tile* tile, GameContext G) const {
    string info;
    PropertyTile* property = dynamic_cast<PropertyTile*>(tile);
    
    if (property != nullptr && property->isOwned() && property->getOwner() != nullptr) {
        // Cari nomor pemain
        int playerNum = 0;
        int counter = 1;
        for (Player& p : G.getPlayers()) {
            if (property->getOwner() == &p) {
                playerNum = counter;
                break;
            }
            counter++;
        }
        
        info += "P" + to_string(playerNum);
        
        // Cek rumah/hotel jika street
        StreetTile* street = dynamic_cast<StreetTile*>(property);
        if (street != nullptr) {
            if (street->getHasHotel()) {
                info += " *";
            } else {
                int houseCount = street->getHouseCount();
                if (houseCount > 0) {
                    info += " " + string(houseCount, '^');
                }
            }
        }
    }
    
    return info;
}

// Build info posisi pemain: (1,3) atau IN:2 V:1
string DisplayView::buildPlayerPositionInfo(int tileIdx, GameContext G) const {
    vector<Player*> onTile = getPlayersOnTile(tileIdx, G);
    string info;
    
    if (onTile.empty()) {
        return info;
    }
    
    // Khusus tile penjara (PEN)
    Tile* tile = G.getBoard().getTile(tileIdx);
    if (tile != nullptr && tile->getCode() == "PEN") {
        vector<int> inJail, visiting;
        int counter = 1;
        
        for (Player& p : G.getPlayers()) {
            bool isOnTile = (p.getPosition() == tileIdx);
            if (isOnTile) {
                if (p.getStatus() == JAILED) {
                    inJail.push_back(counter);
                } else {
                    visiting.push_back(counter);
                }
            }
            counter++;
        }
        
        if (!inJail.empty()) {
            info += "IN:";
            for (size_t i = 0; i < inJail.size(); ++i) {
                if (i > 0) info += ",";
                info += to_string(inJail[i]);
            }
        }
        if (!visiting.empty()) {
            if (!inJail.empty()) info += " ";
            info += "V:";
            for (size_t i = 0; i < visiting.size(); ++i) {
                if (i > 0) info += ",";
                info += to_string(visiting[i]);
            }
        }
    } else {
        // Tile biasa: tampilkan (1,3)
        info += "(";
        int counter = 1;
        bool first = true;
        for (Player& p : G.getPlayers()) {
            if (p.getPosition() == tileIdx) {
                if (!first) info += ",";
                info += to_string(counter);
                first = false;
            }
            counter++;
        }
        info += ")";
    }
    
    return info;
}

// Baris pertama tile: [COLOR] CODE
string DisplayView::getTileLine1(Tile* tile) const {
    if (tile == nullptr) {
        return formatCellContent("");
    }
    
    string color = normalizeTileColorCode(tile);
    
    string code = tile->getCode();
    if ((int)code.size() > 3) {
        code = code.substr(0, 3);
    }
    
    return formatCellContent("[" + color + "] " + code);
}

// Baris kedua tile: ownership + player positions
string DisplayView::getTileLine2(int tileIdx, Tile* tile, GameContext G) const {
    string line = buildOwnershipInfo(tile, G);
    
    string playerInfo = buildPlayerPositionInfo(tileIdx, G);
    if (!playerInfo.empty()) {
        if (!line.empty()) {
            line += " ";
        }
        line += playerInfo;
    }
    
    return formatCellContent(line);
}

// Print 11 tile baris atas (index 0-10)
void DisplayView::printTopRow(GameContext G, int sideLength) {
    string border = makeBorderLine(sideLength);
    cout << border << endl;
    
    // Baris pertama: [COLOR] CODE
    for (int i = 0; i < sideLength; ++i) {
        Tile* t = G.getBoard().getTile(i);
        cout << "|" << colorizeText(getTileLine1(t), normalizeTileColorCode(t));
    }
    cout << "|" << endl;
    
    // Baris kedua: ownership + players
    for (int i = 0; i < sideLength; ++i) {
        Tile* t = G.getBoard().getTile(i);
        cout << "|" << getTileLine2(i, t, G);
    }
    cout << "|" << endl;
    cout << border << endl;
}

// Print 11 tile baris bawah (index 30 ke 20)
void DisplayView::printBottomRow(GameContext G, int sideLength) {
    string border = makeBorderLine(sideLength);
    int bottomLeft = 3 * (sideLength - 1);
    int bottomRight = 2 * (sideLength - 1);

    for (int i = bottomLeft; i >= bottomRight; --i) {
        Tile* t = G.getBoard().getTile(i);
        cout << "|" << colorizeText(getTileLine1(t), normalizeTileColorCode(t));
    }
    cout << "|" << endl;
    
    for (int i = bottomLeft; i >= bottomRight; --i) {
        Tile* t = G.getBoard().getTile(i);
        cout << "|" << getTileLine2(i, t, G);
    }
    cout << "|" << endl;
    cout << border << endl;
}

// Print 9 baris sisi (kiri dan kanan)
void DisplayView::printSideRows(GameContext G, int sideLength, int centerWidth) {
    int totalTiles = G.getBoard().getTotalTile();
    int topRight = sideLength - 1;
    int sideRows = sideLength - 2;

    vector<string> centerLines = buildCenterLines(G, centerWidth, sideRows);
    string sideBorder = makeSideBorderLine(centerWidth);
    
    for (int row = 1; row <= sideRows; ++row) {
        int leftIdx = totalTiles - row;
        int rightIdx = topRight + row;
        
        Tile* leftTile = G.getBoard().getTile(leftIdx);
        Tile* rightTile = G.getBoard().getTile(rightIdx);
        
        // Baris pertama: [COLOR] CODE
        string leftCell1 = colorizeText(getTileLine1(leftTile), normalizeTileColorCode(leftTile));
        string rightCell1 = colorizeText(getTileLine1(rightTile), normalizeTileColorCode(rightTile));
        int firstCenterIndex = (row - 1) * 2;
        cout << "|" << leftCell1 << "|" << centerLines[firstCenterIndex] << "|" << rightCell1 << "|" << endl;
        
        // Baris kedua: ownership + players
        string leftCell2 = getTileLine2(leftIdx, leftTile, G);
        string rightCell2 = getTileLine2(rightIdx, rightTile, G);
        int secondCenterIndex = firstCenterIndex + 1;
        cout << "|" << leftCell2 << "|" << centerLines[secondCenterIndex] << "|" << rightCell2 << "|" << endl;
        
        if (row != sideRows) {
            cout << sideBorder << endl;
        }
    }
}

void DisplayView::renderBoard(GameContext G) {
    int sideLength = getBoardSideLength(G);
    if (sideLength < 3) {
        cout << "Board tidak bisa dirender dinamis: jumlah tile harus kelipatan 4 dan minimal 8." << endl;
        return;
    }

    int centerWidth = getBoardCenterWidth(sideLength);
    printTopRow(G, sideLength);
    printSideRows(G, sideLength, centerWidth);
    printBottomRow(G, sideLength);
}


void DisplayView::renderAkta(GameContext G, string code){
    Tile* tile = G.getBoard().getTileByCode(code);

    if (tile == nullptr) {
        printCardLine("Tile " + code + " is not found");
        return;
    }
    
    PropertyTile* prop = dynamic_cast<PropertyTile*>(tile);
    if (prop == nullptr) {
        printCardLine("Tile " + code + " is not a property tile");
        return;
    }
    
    cout << "+==============================+" << endl;
    printCardLine("AKTA KEPEMILIKAN");
    printCardLine("[" + prop->getColor() + "] " + prop->getName() + " (" + prop->getCode() + ")");
    cout << "+==============================+" << endl;
    printCardLine("Buy Price        : M" + to_string(prop->getPrice()));
    printCardLine("Mortgage Value   : M" + to_string(prop->getMorgageValue()));
    cout << "+------------------------------+" << endl;
    
    vector<string> detailLines = prop->getAktaDetailLines(G);
    for (const string& line : detailLines) {
        if (line == "-") {
            cout << "+------------------------------+" << endl;
        } else {
            printCardLine(line);
        }
    }

    cout << "+==============================+" << endl;
    if (prop->isOwned()) {
        Player* owner = prop->getOwner();
        printCardLine("Status: OWNED (" + owner->getName() + ")");
    }else if(prop->getStatus() == MORTGAGED) {
        printCardLine("Status: MORTGAGED (" + prop->getOwner()->getName() + ")");
    } else {
        printCardLine("Status: BANK");
    }
    cout << "+==============================+" << endl;
}

void DisplayView::renderBuyStreet(GameContext G, StreetTile* tile){
    vector<int> rentPrices = tile->getRentPrices();
    cout << "+==============================+" << endl;
    printCardLine("[" + tile->getColor() + "] " + tile->getName() + " (" + tile->getCode() + ")");
    printCardLine("Buy Price: M" + to_string(tile->getPrice()));
    printCardLine("Rent Fee: M" + to_string(rentPrices[0]));
    cout << "+==============================+" << endl;
    cout << "Your current balance: M" << G.getCurrentPlayer().getBalance() << endl;
    cout << "Do you want to buy this property? (y/n): ";
}

void DisplayView::renderBoughtStreet(GameContext G, StreetTile* tile){
    cout << "You have bought " << tile->getName() << "!" << endl;
    cout << "Your new balance: M" << G.getCurrentPlayer().getBalance() << endl;
}

void DisplayView::renderGetRailroad(GameContext G, RailroadTile* tile){
    renderTile(G);
    cout << "Not owned by anyone, this railroad is now yours!" << endl;
}

void DisplayView::renderGetUtility(GameContext G, UtilityTile* tile){
    renderTile(G);
    cout << "Not owned by anyone, " << tile->getName() << " is now yours!" << endl;
}

void DisplayView::renderProperty(GameContext G){
    Player* current = &G.getCurrentPlayer();
    
    map<string, vector<PropertyTile*>> ownedProperties; // key: warna, value: pointer ke property tile yang dimiliki
    int totalTile = G.getBoard().getTotalTile();
    for (int i = 0; i < totalTile; ++i) {
        Tile* tile = G.getBoard().getTile(i);
        PropertyTile* prop = dynamic_cast<PropertyTile*>(tile);
        if (prop != nullptr && prop->getOwner() == current) {
            ownedProperties[prop->getColor()].push_back(prop);
        }
    }
    
    if (ownedProperties.empty()) {
        cout << "You haven't bought any properties yet." << endl;
        return;
    }
    
    cout << "=== Properties Owned by " + current->getName() + " ===" << endl;
    for (const auto& entry : ownedProperties) {
        const vector<PropertyTile*>& props = entry.second;
        cout << "[" << entry.first << "]" << endl;

        for (PropertyTile* prop : props) {
            cout << "- " << prop->getPropertyDisplayInfo() << endl;
        }   
    }

    cout << "Total Wealth: M" << current->totalWealth() << endl;
}


void DisplayView::renderTile(GameContext G){
    cout << "You are on tile: " << G.getBoard().getTile(G.getCurrentPlayer().getPosition())->getName() << "(" << G.getBoard().getTile(G.getCurrentPlayer().getPosition())->getCode() << ")" << endl;
}

void DisplayView::showMenu(GameContext G){
    
}

void DisplayView::renderDiceRoll(GameContext G){
    cout << "Rolling dice..." << endl;
    Dice dice = G.getDice();
    cout << "You rolled a " << dice.getDice1() << " + " << dice.getDice2() << " = " << dice.getTotal() << endl;
    cout << "Moving " << G.getCurrentPlayer().getName() << " " << dice.getTotal() << " spaces." << endl;
    cout << "You landend on tile: " << G.getBoard().getTile(G.getCurrentPlayer().getPosition())->getName() << "." << endl;
}

void DisplayView::renderDiceControl(GameContext G){
    cout << "You manually set the dice values." << endl;
    Dice dice = G.getDice();
    cout << "You rolled a " << dice.getDice1() << " + " << dice.getDice2() << " = " << dice.getTotal() << endl;
    cout << "Moving " << G.getCurrentPlayer().getName() << " " << dice.getTotal() << " spaces." << endl;
    cout << "You landend on tile: " << G.getBoard().getTile(G.getCurrentPlayer().getPosition())->getName() << "." << endl;
}

void DisplayView::renderRent(GameContext G, PropertyTile* tile){
    cout << "You need to pay rent for landing on " << tile->getName() << "." ;
    Player* owner = tile->getOwner();
    if (owner != nullptr) {
        cout << "Owned by " << owner->getName() << "." << endl;
    }
    
    for (string line : tile->getRentDetailLines(G)) {
        cout << line << endl;
    }
}

void DisplayView::renderMortgage(GameContext G, PropertyTile* tile){
    cout << "You have mortgaged " << tile->getName() << "(" << tile->getCode() << "),";
    cout << "Owned by " << tile->getOwner()->getName() << "." << endl;
    cout << "This property is now mortgaged [M]. No rent will be collected from this property." << endl;
}

void DisplayView::renderCantPay(GameContext G, int amountOwed){
    cout << "You can't pay the required amount of rent." << "(M" << amountOwed << ")" << endl;
    cout << "Your current balance: M" << G.getCurrentPlayer().getBalance() << endl;
}

void DisplayView::renderTax(GameContext G, TaxTile* tile){
    cout << "You landed on a tax tile " << tile->getName() << " (" << tile->getCode() << ")." << endl;
    cout << "Choose one of the following tax payment options:" << endl;
    cout << "1. Pay a fixed amount of M" << G.getPph() << endl;
    cout << "2. Pay " << G.getTaxRate() * 100 << "% of your total wealth" << endl;
    cout << "Enter your choice (1 or 2): ";
}   

void DisplayView::renderPayTax()