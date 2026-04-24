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
    printCardLine("Mortgage Value   : M" + to_string(prop->getMortgageValue()));
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
    cout << "You landed on " << G.getBoard().getTile(G.getCurrentPlayer().getPosition())->getName() << "(" << G.getBoard().getTile(G.getCurrentPlayer().getPosition())->getCode() << ")" << endl;
}

void DisplayView::showMenu(GameContext G){
    
}

void DisplayView::renderDiceRoll(GameContext G){
    cout << "Rolling dice..." << endl;
    Dice dice = G.getDice();
    cout << "You rolled a " << dice.getDice1() << " + " << dice.getDice2() << " = " << dice.getTotal() << endl;
    cout << "Moving " << G.getCurrentPlayer().getName() << " " << dice.getTotal() << " spaces." << endl;
    cout << "You landed on tile: " << G.getBoard().getTile(G.getCurrentPlayer().getPosition())->getName() << "." << endl;
}

void DisplayView::renderDiceControl(GameContext G){
    cout << "You manually set the dice values." << endl;
    Dice dice = G.getDice();
    cout << "You rolled a " << dice.getDice1() << " + " << dice.getDice2() << " = " << dice.getTotal() << endl;
    cout << "Moving " << G.getCurrentPlayer().getName() << " " << dice.getTotal() << " spaces." << endl;
    renderTile(G);
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
    if (tile->getIsPPH()) {
        cout << "You landed on tax tile " << tile->getName() << " (" << tile->getCode() << ")." << endl;
        cout << "Choose one of the following tax payment options:" << endl;
        cout << "1. Pay a fixed amount of M" << G.getPphFlat() << endl;
        cout << "2. Pay " << G.getPphPercentage() << "% of your total wealth" << endl;
        cout << "Enter your choice (1 or 2): ";
    } else {
        cout << "You landed on a tax tile " << tile->getName() << " (" << tile->getCode() << ")." << endl;
        cout << "You must pay a fixed amount of M" << G.getPbm() << "." << endl;
        cout << "Your Balance : M" << G.getCurrentPlayer().getBalance() << "-> M" << G.getCurrentPlayer().getBalance() - G.getPbm() << endl;
    }
}   

void DisplayView::renderPayTax(GameContext G, int choose){
    if (choose == 2) {
        cout << "Total wealth: " << G.getCurrentPlayer().totalWealth() << endl;
        cout << "- Cash Balance : M" << G.getCurrentPlayer().getBalance() << endl;
        cout << "- Property Price : M" << G.getCurrentPlayer().totalPropertyPrice() << endl;
        cout << "- Building Value : M" << G.getCurrentPlayer().totalBuildingValue() << endl;
        cout << "Total : M" << G.getCurrentPlayer().totalWealth() << endl;
        int taxAmount = (G.getPphPercentage() * G.getCurrentPlayer().totalWealth()) / 100;
        if (taxAmount > G.getCurrentPlayer().getBalance()) {
            cout << "However, you don't have enough balance to pay this tax M" << taxAmount << endl;
            cout << "Your Balance : M" << G.getCurrentPlayer().getBalance() << endl;
        } else {
            cout << "You chose to pay " << G.getPphPercentage() << "% of your total wealth." << endl;
            cout << "You paid M" << taxAmount << " in taxes." << endl;
            cout << "Your Balance : M" << G.getCurrentPlayer().getBalance() << "-> M" << G.getCurrentPlayer().getBalance() - taxAmount << endl;
        }
    } else {
        // Default to option 1 if input is invalid
        cout << "You chose to pay a fixed amount of M" << G.getPphFlat() << "." << endl;
        if (G.getPphFlat() > G.getCurrentPlayer().getBalance()) {
            cout << "However, you don't have enough balance to pay this tax M" << G.getPphFlat() << endl;
            cout << "Your Balance : M" << G.getCurrentPlayer().getBalance() << endl;
        } else {
            cout << "You paid M" << G.getPphFlat() << " in taxes." << endl;
            cout << "Your Balance : M" << G.getCurrentPlayer().getBalance() << "-> M" << G.getCurrentPlayer().getBalance() - G.getPphFlat() << endl;
        }
    }
}


// RENDER LELANG

// USAGE :
// 1. Panggil renderAuctionLine(playerName) untuk menampilkan prompt lelang
// 2. Panggil Input handler
// 3. Setelah input diterima, panggil HighestBidder(playerName, bidAmount) untuk menampilkan penawaran tertinggi saat ini
// 4. Ulangi langkah 1-3 sampai lelang selesai
// 5. 

void DisplayView::renderAuctionStart(GameContext G, PropertyTile* tile){
    cout << "Auction started for " << tile->getName() << " (" << tile->getCode() << ")!" << endl;
    cout << "Auction start from player after current player: " << G.getPlayers()[(G.getCurrentPlayerIndex() + 1) % G.getPlayers().size()].getName() << endl;
}

void DisplayView::renderAuctionLine(string playerName){
    cout << "Turn : " << playerName << endl;
    cout << "Enter your bid (BID <amount> / PASS): ";
}

void DisplayView::HighestBidder(string playerName, int bidAmount){
    cout << "The Highest bid : " << bidAmount << "(" << playerName << ")" << endl;
}

void DisplayView::renderAuctionResult(string winnerName, int winningBid){
    cout << "Auction Finished!" << endl;
    cout << "Winner : " << winnerName << endl;
    cout << "Winning Bid : M" << winningBid << endl;
}

// Festival 
void DisplayView::renderFestivalTile(GameContext G, FestivalTile *tile) {
    renderTile(G);
    if (G.getCurrentPlayer().getOwnedProperties().empty()) {
        cout << "You don't have any properties to apply the festival on." << endl;
        return;
    }

    cout << "Your owned properties:" << endl;
    for (const PropertyTile* Tile : G.getCurrentPlayer().getOwnedProperties())
    {
        cout << "- " << Tile->getCode() << " (" << Tile->getName() << ")" << endl;
    }

    cout << "Insert code property: ";
}

void DisplayView::InputUnvalidFestivalProperty(GameContext G){
    cout << "Invalid property code. Please enter a valid property code from your owned properties." << endl;
    cout << "Your owned properties:" << endl;
    for (const PropertyTile* Tile : G.getCurrentPlayer().getOwnedProperties())
    {
        cout << "- " << Tile->getCode() << " (" << Tile->getName() << ")" << endl;
    }
    cout << "Insert code property: ";
}

void DisplayView::renderFestivalResult(GameContext G, StreetTile* tile) {
    int rentPrice;
    if (tile->getHasHotel()) {
        rentPrice = tile->getRentPrices()[5]; // Rent untuk hotel
    } else {
        int houseCount = tile->getHouseCount();
        rentPrice = tile->getRentPrices()[houseCount];
    }

    if (tile->isFestivalActive()) {
        cout << "Festival activated!" << endl << endl;
        cout << "First Rent: M" << rentPrice << endl;
        cout << "After Festival rent : M" << rentPrice * tile->getFestivalStack() << endl;
        cout << "Duration : " << tile->getFestivalTurn() << " turns" << endl;
    }else 
    {
        cout << "Festival Upgraded!" << endl << endl;
        cout << "First Rent: M" << rentPrice << endl;
        cout << "After Festival rent : M" << rentPrice * tile->getFestivalStack() << endl;
        cout << "Duration Resets: " << tile->getFestivalTurn() << " turns" << endl;
    }
}

// Redeem Mortgage
void DisplayView::renderRedeemStart(GameContext G, vector<PropertyTile*> mortgagedTiles){
    if (mortgagedTiles.empty()) {
        cout << "You don't have any mortgaged properties to redeem." << endl;
        return;
    }
    cout << "=== PROPERTY MORTGAGED ===" << endl;
    int i = 1;
    for (const PropertyTile* tile : mortgagedTiles)
    {
        if (tile->getStatus() == MORTGAGED){
            cout << i << ". " << tile->getName() << " (" << tile->getCode() << ")" << "[ << " << tile->getColor() << " >> ]" << "[M]"  << "Mortgage Value: M" << tile->getPrice() << endl;
            i++;
        }
    }
    cout << "Your balance: M" << G.getCurrentPlayer().getBalance() << endl;
    cout << "Choose a property to unmortgage (0 to skip): ";
    
}

// Mortgage 
void DisplayView::renderMortgageStart(GameContext G, vector<PropertyTile*> unmortgagedTiles){
    if (unmortgagedTiles.empty()) {
        cout << "You don't have any properties to mortgage." << endl;
        return;
    }
    cout << "=== PROPERTY UNMORTGAGED ===" << endl;
    int i = 1;
    for (const PropertyTile* tile : unmortgagedTiles)
    {
        if (tile->getStatus() != MORTGAGED){
            cout << i << ". " << tile->getName() << " (" << tile->getCode() << ")" << "[ << " << tile->getColor() << " >> ]"  << "Mortgage Value: M" << tile->getMortgageValue() << endl;
            i++;
        }
    }
    cout << "Choose a property to mortgage (0 to skip): ";
}
void DisplayView::renderMortgageResult(GameContext G, PropertyTile* tile){
    cout << "You have mortgaged " << tile->getName() << "(" << tile->getCode() << "),";
    cout << "You receive M" << tile->getMortgageValue() << " from the bank." << endl;
    cout << "Your current balance: M" << G.getCurrentPlayer().getBalance() << endl;
    cout << "This property is now mortgaged [M]. No rent will be collected from this property." << endl;
}

void DisplayView::renderMortgageGroupColorStart(GameContext G, vector<StreetTile*> sameColorProps){
    cout << "There's still building in color group " << sameColorProps[0]->getColor() << endl << endl;

    cout << "List of properties in the same color group:" << endl;
    int i = 1;
    for (const StreetTile* tile : sameColorProps)
    {
        if (tile->getHouseCount() > 0 || tile->getHasHotel()){
            continue;
        }

        if (tile->getStatus() == MORTGAGED){
            continue;
        }
        if (tile->getHasHotel()){
            cout << i << ". " << tile->getName() << " (" << tile->getCode() << ") " << "- " << tile->getHouseCount() << endl;
        } else {
            cout << i << ". " << tile->getName() << " (" << tile->getCode() << ") " << "- " << tile->getHouseCount() << endl;
        }
        i++;
    }   
    cout << "Sell all building in the color group [" << sameColorProps[0]->getColor() << "]: (Y/N)" << endl;
}

void DisplayView::renderMortgageGroupColorResult(GameContext G, string choice, vector<StreetTile*> sameColorProps){
    if (choice != "y" && choice != "Y") {
        cout << "Mortgage cancelled. You cannot mortgage this property until you sell all buildings in the color group." << endl;
        return;
    }else{
        int totalValue = 0;
        for (StreetTile* tile : sameColorProps) {
            if (tile->getHouseCount() > 0 || tile->getHasHotel()){
                cout << "All buildings on " << tile->getName() << " have been sold.";
                cout << "You receive M" << tile->getBuildingValue() / 2 << endl;
                totalValue += tile->getBuildingValue() / 2;
            }
        }
        cout << "Your current balance: M" << G.getCurrentPlayer().getBalance() + totalValue << endl;
    }
}

void DisplayView::renderRedeemChoose(GameContext G, vector<PropertyTile*> mortgagedTiles, int choose){
    if (choose == 0) {
        cout << "You chose not to unmortgage any property." << endl;
        return;
    }

    if (choose < 1 || choose > (int)mortgagedTiles.size()) {
        cout << "Invalid choice. Please choose a valid property number." << endl;
        return;
    }

    PropertyTile* chosenTile = mortgagedTiles[choose - 1];
    // Jaga - jaga agar tidak terjadi error jika status tile berubah di tengah proses
    if (chosenTile->getStatus() != MORTGAGED) {
        cout << "The chosen property is not currently mortgaged." << endl;
        return; 
    }

    int unmortgageCost = chosenTile->getPrice();
    if (G.getCurrentPlayer().getBalance() < unmortgageCost) {
        cout << "You don't have enough balance to unmortgage this property." << endl; 
        cout << "Required: M" << unmortgageCost << "| Your Balance: M" << G.getCurrentPlayer().getBalance() << endl;
        return;
    }

    cout << "You chose to unmortgage " << chosenTile->getName() << " (" << chosenTile->getCode() << ")." << endl;
    cout << "You paying M" << unmortgageCost << "To the bank" << endl;
    cout << "Your Current Balance: M" << G.getCurrentPlayer().getBalance() << " -> M" << G.getCurrentPlayer().getBalance() - unmortgageCost << endl;
}

// Build 
void DisplayView::renderBuildStart(GameContext G, map<string, vector<StreetTile*>> tiles){
    cout << "== Qualified Color Groups ==" << endl;
    int i = 1;
    map<string, vector<StreetTile*>> properties = G.getBoard().getMapColorProperty();

    for (const auto& entry : tiles) {
        const vector<StreetTile*>& streets = entry.second;

        if (properties[entry.first].size() != streets.size()) {
            continue; // Skip color groups that are not fully owned
        }

        cout << i << ". [" << entry.first << "]" << endl;
        for (StreetTile* tile : streets) {
            if(tile->getHasHotel()) continue;
            string buildingInfo = tile->getHouseCount() > 0 ? to_string(tile->getHouseCount()) + " Houses" : "No Houses";
            cout << "   - " << tile->getName() << " (" << tile->getCode() << "): " << buildingInfo << "(House cost: M" << tile->getHouseCost() << ")" << endl;
        }
        i++;
    }

    cout << "Your Balance: M" << G.getCurrentPlayer().getBalance() << endl;
    cout << "Choose a color group to build on (0 to skip): ";
}

void DisplayView::renderBuildMid(GameContext G, vector<StreetTile*> buildableTiles){
    cout << "Color Group: " << "[" << buildableTiles[0]->getColor() << "]" << endl;
    int maxHouseCount = (*max_element(
        buildableTiles.begin(),
        buildableTiles.end(),
        [](StreetTile* a, StreetTile* b) {
            return a->getHouseCount() < b->getHouseCount();
        }
    ))->getHouseCount();

    int minHouseCount = (*min_element(
        buildableTiles.begin(),
        buildableTiles.end(),
        [](StreetTile* a, StreetTile* b) {
            return a->getHouseCount() < b->getHouseCount();
        }
    ))->getHouseCount();

    renderStreetBuilt(buildableTiles, maxHouseCount, minHouseCount);

    if (minHouseCount == 4 && maxHouseCount == 4) {
        cout << "All properties on this color group [" << buildableTiles[0]->getColor() << "] have 4 houses. You can choose to upgrade to a hotel." << endl;
    }

    cout << "Choose a property to build on (0 to skip): ";
}

void DisplayView::renderStreetBuilt(vector<StreetTile*> tiles, int maxHouseCount, int minHouseCount){
    for (StreetTile* streetTile : tiles){
    
        string buildableInfo;
        string buildingInfo = streetTile->getHouseCount() > 0 ? to_string(streetTile->getHouseCount()) + " Houses" : "No Houses";
        
        if (streetTile->getHasHotel()) {
            buildableInfo = "Can't Build (Already has Hotel)";
            buildingInfo = "Hotel";
        }

        
        if (streetTile->getStatus() == MORTGAGED) {
            buildableInfo = "Can't Build (Property is Mortgaged)";
        }
        
        if (minHouseCount != maxHouseCount && streetTile->getHouseCount() == minHouseCount) {
            buildableInfo = streetTile->getHouseCount() == minHouseCount ? "Can Build" : "Can't Build (Must build evenly)";
        }
        
        if (streetTile->getHouseCount() == 4 && !streetTile->getHasHotel() && minHouseCount == 4) {
            buildableInfo = "Ready to Upgrade to Hotel";
        }

        cout << "- " << streetTile->getName() << " (" << streetTile->getCode() << "): " << buildingInfo << " <- " << buildableInfo << endl;
    }
}
void DisplayView::renderBuildCancel(GameContext G){
    cout << "You chose not to build on any property." << endl;
}

void DisplayView::renderBuildInvalid(GameContext G){
    cout << "Invalid choice. Please choose a valid property number." << endl;
}

void DisplayView::renderbuildHouses(GameContext G, StreetTile* tile, vector<StreetTile*> buildableTiles){
     if (tile->getHasHotel()) {
        cout << "You have upgraded " << tile->getName() << " to a Hotel!" << endl;
    } else {
        cout << "You have built a house on " << tile->getName() << "!" << endl;
    }
    cout << "Your new balance: M" << G.getCurrentPlayer().getBalance() << endl;
    for (const StreetTile* streetTile : buildableTiles)
    {
        string buildingInfo = streetTile->getHouseCount() > 0 ? to_string(streetTile->getHouseCount()) + " Houses" : "No Houses";
        if (streetTile->getHasHotel()) {
            buildingInfo = "Hotel";
        }
        cout << "- " << streetTile->getName() << " (" << streetTile->getCode() << "): " << buildingInfo << endl;
    }
}

// SaveLoad
void DisplayView::renderSaveSuccess(GameContext G, string filename){
    cout << "Saving game to ..." << endl;
    cout << "Game successfully saved to file: " << filename << endl;
}

void DisplayView::renderSaveFilenameUsed(GameContext G, string filnename){
    cout << "Filename '" << filnename << "' is already used. " << endl;
    cout << "Do you want to overwrite the existing file? (y/n): ";
}

void DisplayView::renderOverwrite(GameContext G, string filename, string choice){
    if (choice != "y" && choice != "Y") {
        cout << "Save operation cancelled. Your game was not saved." << endl;
        return;
    }else{
        cout << "Overwriting file '" << filename << "'..." << endl;
        cout << "Game successfully saved to file: " << filename << endl;
    }
}

void DisplayView::renderSaveFailure(GameContext G, string filename){
    cout << "Failed to save game to file: " << filename << endl;
    cout << "Please check if the filename is valid and try again." << endl;
}


void DisplayView::renderLoadSuccess(GameContext G, string filename){
    cout << "Loading game from file: " << filename << "..." << endl;
    cout << "Game successfully loaded from file: " << filename << endl;
}

void DisplayView::renderLoadFailure(GameContext G, string filename, bool fileExists){
    if (fileExists) {
        cout << "Failed to load game from file: " << filename << endl;
        cout << "Please check if the file is valid and try again." << endl;
    } else {
        cout << "File not found: " << filename << endl;
        cout << "Please check the filename and try again." << endl;
    }
}

void DisplayView::renderGameOverMaxTurn(GameContext G){
    cout << "Game Over! " << endl;  
    cout << "Final Standings:" << endl;
    vector<Player> players = G.getPlayers();
    for (size_t i = 0; i < players.size(); ++i) {
        renderPlayerInfo(G, &players[i]);
    }
    
    sort(players.begin(), players.end(), [](const Player& a, const Player& b) {
        return a.totalWealth() > b.totalWealth();
    });

    cout << "Winner: " << players[0].getName() << " with total wealth of M" << players[0].totalWealth() << "!" << endl;
}

void DisplayView::renderPlayerInfo(GameContext G, Player* player){
    cout << "=== Player Info: " << player->getName() << " ===" << endl;
    cout << "Balance: M" << player->getBalance() << endl;
    if (!player->getOwnedProperties().empty()) {
        cout << "Properties Owned: " << player->getOwnedProperties().size() << endl;
    }
    if (player->getSkillCardCount() > 0) {
        cout << "Skill Cards: " << player->getSkillCardCount() << endl;
    }
    cout << "" << endl;
}

void DisplayView::renderPlayer(GameContext G){
    for (Player& p : G.getPlayers()) {
        if (p.getStatus() != PlayerStatus::BANKRUPT) {
            cout << "- " << p.getName() << " (Currently at tile: " << p.getPosition() << ")\n";
        }
    }
}

// Bankrupt
void DisplayView::renderBankruptFirstSceneRent(GameContext G, Player* bankruptPlayer, Player* creditorPlayer, int amountToPay){
    cout << "You can't pay the required rent M" << amountToPay << " to " << creditorPlayer->getName() << "." << endl << endl;
    cout << "Your current balance: M" << bankruptPlayer->getBalance() << endl;
    cout << "Amount owed: M" << amountToPay << endl;
    cout << "Your shortfall: M" << amountToPay - bankruptPlayer->getBalance() << endl;

    int totalAssetValue = 0;
    for (PropertyTile* tile : bankruptPlayer->getOwnedProperties()) {
        if (tile->getStatus() == MORTGAGED) {
            cout << "   Mortgage " << tile->getName() << " (" << tile->getCode() << ") [" << tile->getColor() << "] -> M" << tile->getMortgageValue() << endl;
            totalAssetValue += tile->getMortgageValue();
        } else {
            if (StreetTile* streetTile = dynamic_cast<StreetTile*>(tile)) {
                cout << "   Sell " << tile->getName() << " (" << tile->getCode() << ") [" << tile->getColor() << "] -> M" << tile->getPrice() + streetTile->getBuildingValue() / 2 << " (Building Value: M" << streetTile->getBuildingValue() / 2 << ")" << endl;
                totalAssetValue += tile->getPrice() + streetTile->getBuildingValue() / 2;
            } else {    
                cout << "   Sell " << tile->getName() << " (" << tile->getCode() << ") [" << tile->getColor() << "] -> M" << tile->getMortgageValue() << endl;
                totalAssetValue += tile->getMortgageValue();
            }
        }
    }
    cout << "   Potential Liquidation Value -> M" << totalAssetValue << endl;

    cout << "You can pay your debt by liquidating your assets." << endl;
    cout << "You should liquidate assets to pay off your debt." << endl;
}

void DisplayView::renderBankruptFirstSceneTax(GameContext G, Player* bankruptPlayer, TaxTile* bankruptTile, int amountToPay){
    cout << "You failed to pay " << bankruptTile->getName() << "M" << amountToPay << endl << endl;
    cout << "Your current balance: M" << bankruptPlayer->getBalance() << endl;
    cout << "Amount owed: M" << amountToPay << endl;
    cout << "Your shortfall: M" << amountToPay - bankruptPlayer->getBalance() << endl;

    int totalAssetValue = 0;
    for (PropertyTile* tile : bankruptPlayer->getOwnedProperties()) {
        if (tile->getStatus() == MORTGAGED) {
            cout << "   Mortgage " << tile->getName() << " (" << tile->getCode() << ") [" << tile->getColor() << "] -> M" << tile->getMortgageValue() << endl;
            totalAssetValue += tile->getMortgageValue();
        } else {
            if (StreetTile* streetTile = dynamic_cast<StreetTile*>(tile)) {
                cout << "   Sell " << tile->getName() << " (" << tile->getCode() << ") [" << tile->getColor() << "] -> M" << tile->getPrice() + streetTile->getBuildingValue() / 2 << " (Building Value: M" << streetTile->getBuildingValue() / 2 << ")" << endl;
                totalAssetValue += tile->getPrice() + streetTile->getBuildingValue() / 2;
            } else {
                cout << "   Sell " << tile->getName() << " (" << tile->getCode() << ") [" << tile->getColor() << "] -> M" << tile->getMortgageValue() << endl;
                totalAssetValue += tile->getMortgageValue();
            }
        }
    }
    cout << "   Potential Liquidation Value -> M" << totalAssetValue << endl;

    cout << "You can pay your debt by liquidating your assets." << endl;
    cout << "You should liquidate assets to pay off your debt." << endl;
}

void DisplayView::liquidatePanel(GameContext G, Player* bankruptPlayer, Player* creditorPlayer, int amountToPay){
    cout << "=== Liquidation Panel ===" << endl;
    cout << "Your current balance: M" << bankruptPlayer->getBalance() << "| Amount owed: M" << amountToPay << endl;
    int i = 1;
    cout << "[Sell to BANK]" << endl;
    for (PropertyTile* tile : bankruptPlayer->getOwnedProperties()) {
        if (tile->getStatus() == OWNED){
            string buildingInfo = "";
            
            if (StreetTile* streetTile = dynamic_cast<StreetTile*>(tile)) {
                buildingInfo = " with :" + to_string(streetTile->getHouseCount()) + " houses";
                if (streetTile->getHasHotel()) {
                    buildingInfo = " a hotel";
                }
                cout << i << "." << streetTile->getName() << " (" << streetTile->getCode() << ") [" << streetTile->getColor() << "] Sell Price: M" << streetTile->getPrice() + streetTile->getBuildingValue() / 2 << "(" << buildingInfo << ": M" << streetTile->getBuildingValue() << ")" << endl;
            }else{
                cout << i << "." << tile->getName() << " (" << tile->getCode() << ") [" << tile->getColor() << "] Sell Price: M" << tile->getMortgageValue() << endl;

            }
            i++;
        }
    }

    cout << "Mortgage Properties" << endl;
    for (PropertyTile* tile : bankruptPlayer->getOwnedProperties()) {
        if (tile->getStatus() == MORTGAGED){
            cout << i << "." << tile->getName() << " (" << tile->getCode() << ") [" << tile->getColor() << "] Mortgage Value: M" << tile->getMortgageValue() << endl;
            i++;
        }
    }

    cout << "Choose an asset to liquidate (0 if enough): ";
}

void DisplayView::renderLiquidateChoose(GameContext G, Player* bankruptPlayer, PropertyTile* chosenTile){
    if (chosenTile->getStatus() == OWNED) {
        if (StreetTile* streetTile = dynamic_cast<StreetTile*>(chosenTile)) {
            int totalValue = chosenTile->getPrice() + streetTile->getBuildingValue() / 2;
            cout << "You sold " << chosenTile->getName() << " to the bank for M" << totalValue << "." << endl;
            cout << "Your new balance: M" << bankruptPlayer->getBalance() + totalValue << endl;
        } else {
            cout << "You sold " << chosenTile->getName() << " to the bank for M"<< chosenTile->getMortgageValue() << "." << endl;
            cout << "Your new balance: M" << bankruptPlayer->getBalance() + chosenTile->getMortgageValue() << endl;
        }
    }
    else if (chosenTile->getStatus() == MORTGAGED)
    {
        cout << "You mortgaged " << chosenTile->getName() << " to the bank for M" << chosenTile->getMortgageValue() << "." << endl;
        cout << "Your new balance: M" << bankruptPlayer->getBalance() + chosenTile->getMortgageValue() << endl;
    }
}

void DisplayView::renderNotEnoughLiquidate(GameContext G, Player* bankruptPlayer, int amountToPay){
    cout << "You still can't pay off your debt after liquidating that asset." << endl;
    cout << "Your current balance: M" << bankruptPlayer->getBalance() << "| Amount owed: M" << amountToPay << endl;
    cout << "You need to liquidate more assets to pay off your debt." << endl;
}

void DisplayView::renderEnoughLiquidate(GameContext G, Player* bankruptPlayer, Player* creditorPlayer, int amountToPay){
    cout << "You have successfully paid off your debt by liquidating your assets." << endl;
    cout << "Your current balance: M" << bankruptPlayer->getBalance() << "-> M" << bankruptPlayer->getBalance() - (amountToPay) << endl;
    cout << creditorPlayer->getName() << " Current Balance: M" << creditorPlayer->getBalance() + amountToPay << endl;
}

void DisplayView::renderBankruptSecondScene(GameContext G, Player* bankruptPlayer, Player* creditorPlayer, PropertyTile* bankruptTile, int amountToPay){
    int totalAssets = bankruptPlayer->totalBuildingValue() / 2 + bankruptPlayer->totalPropertyPrice();
    cout << "You still can't pay the required rent M" << amountToPay << " to " << creditorPlayer->getName() << "." << endl << endl;
    cout << "Your current balance: M" << bankruptPlayer->getBalance() << endl;
    cout << "Amount owed: M" << amountToPay << endl;
    cout << "Your shortfall: M" << amountToPay - bankruptPlayer->getBalance() << endl << endl;

    cout << "Estimation maximum asset value you can liquidate: "<< endl;
    cout << "  Sell all properties + buildings -> M" << totalAssets << endl;
    cout << "Total assets + balance: M" << bankruptPlayer->totalWealth() << endl;
    cout << "Unfortunately, can't pay off your debt M" << amountToPay << "." << endl << endl;

    cout << bankruptPlayer->getName() << " is declared BANKRUPT!" << endl;
    cout << "Creditor " << creditorPlayer->getName();

    cout << " Distributes the assets to " << creditorPlayer->getName() << ":" << endl;
    cout << "  - Receives M" << bankruptPlayer->getBalance() << " in cash." << endl;
    for (PropertyTile* tile : bankruptPlayer->getOwnedProperties()) {
        if (tile->getStatus() == MORTGAGED) {
            cout << "   -" << tile->getName() << " (" << tile->getCode() << ") [" << tile->getColor() << "] MORTGAGED [M]" << endl;
        } else {
            if (StreetTile* streetTile = dynamic_cast<StreetTile*>(tile)) {
                string buildingInfo = streetTile->getHouseCount() > 0 ? to_string(streetTile->getHouseCount()) + " houses" : "no houses";
                if (streetTile->getHasHotel()) {
                    buildingInfo = "a hotel";
                }

                cout << "   -" << tile->getName() << " (" << tile->getCode() << ") [" << tile->getColor() << "] OWNED (" << buildingInfo << ")" << endl;
            }else{
                cout << "   -" << tile->getName() << " (" << tile->getCode() << ") [" << tile->getColor() << "] OWNED"  << endl;
            }
        }
    }
    
    cout << creditorPlayer->getName() << " Acquires all assets of " << bankruptPlayer->getName() << "." << endl;
    cout << bankruptPlayer->getName() << " is removed from the game." << endl;
    cout << "Game continues with " << G.getPlayers().size() - 1 << " players." << endl;
}

void DisplayView::renderBankruptThirdScene(GameContext G, Player* bankruptPlayer, TaxTile* Tile, int amountToPay){
    int totalAssets = bankruptPlayer->totalBuildingValue() / 2 + bankruptPlayer->totalPropertyPrice();
    cout << "You failed to pay " << Tile->getName() << "M" << amountToPay << endl << endl;

    cout << "Estimation maximum asset value you can liquidate: "<< endl;
    cout << "  Sell all properties + buildings -> M" << totalAssets << endl;
    cout << "Total assets + balance: M" << totalAssets + bankruptPlayer->getBalance() << endl;
    cout << "Unfortunately, can't pay off your debt M" << amountToPay << "." << endl << endl;
   
    cout << bankruptPlayer->getName() << " is declared BANKRUPT!" << endl;
    cout << "Creditor : BANK" << endl << endl;

    cout << "Player Balance M" << bankruptPlayer->getBalance() << " has been transferred to the bank." << endl;
    cout << "All properties are returned to the bank and Set to BANK" << endl;
    cout << "Buildings on the properties are removed." << endl;

    cout << "Auction starts for all properties one by one." << endl;
}

void DisplayView::renderPlayerCard(Player& player){
    int displayIdx = 1;
    
    cout << "List of Skill Cards for " << player.getName() << ":\n";
    for (SkillCard* c : player.getSkillCard()) {
        cout << displayIdx << ". " << c->getName() << " - " << c->getDescription() << "\n";
        displayIdx++;
    }
}

void DisplayView::renderReceiveSkillCard(Player& player, SkillCard* card){
    cout << "You received a new skill card!" << endl;
    cout << "You receive: " << card->getName() << "." << endl; 
}

void DisplayView::renderMaxSkillCard(Player& player){
    cout << "ALERT: You have reached the maximum number of skill cards (Maximum 3)." << endl;
    cout << "You should discard one of your existing skill cards to receive the new one." << endl << endl;

    renderPlayerCard(player);

    cout << "Choose a skill card to discard (1 - " << player.getSkillCard().size() << "): ";
}

void DisplayView::renderDiscardSkillCard(Player& player, int choice){
    if (choice < 1 || choice > (int)player.getSkillCard().size()) {
        cout << "Invalid choice. No skill card was discarded." << endl;
        cout << "Choose another skill card to discard (1 - " << player.getSkillCard().size() << "): ";
        return;
    }

    SkillCard* discardedCard = player.getSkillCard()[choice - 1];
    cout << "You discarded: " << discardedCard->getName() << "." << endl;
    cout << "Now you have 3 skill cards in your hand." << endl;
}

void DisplayView::renderCardTile(GameContext G, CardTile* tile, ActionCard* card){
    renderTile(G);
    cout << "You draw a card..." << endl;
    cout << "Card: " << card->getDescription() << endl;
}

void DisplayView::renderActivateCard(GameContext G, Player* player, ActionCard* card){
    cout << card->getName() << " activated!" << card->getDescription() << endl;
}

void DisplayView::renderShieldPay(GameContext G, Player* player, int amountToPay){
    cout << "[SHIELD ACTIVATED]: Shield Effect Protect You!" << endl;
    cout << "Payment for M" << amountToPay << " Cancelled. Your current balance: M" << player->getBalance() << endl;
}

void DisplayView::renderShieldProtect(GameContext G, Player* player){
    cout << "[SHIELD ACTIVATED]: Shield Effect Protect You!" << endl;
    cout << "You are keep in your position and won't be sent to jail. " << endl;
}

void DisplayView::renderDiscountPay(GameContext G, Player* player, int amountToPay, int discountedAmount){
    cout << "[DISCOUNT ACTIVATED]: Discount Effect Applied!" << endl;
    cout << "Original Payment: M" << amountToPay << " -> Discounted Payment: M" << discountedAmount << endl;
    cout << "Your current balance: M" << player->getBalance() << endl;
}

