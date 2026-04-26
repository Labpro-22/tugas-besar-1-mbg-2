#include "BankruptcyController.hpp"
#include "DisplayView.hpp"
#include "AuctionController.hpp"
using namespace std;

vector<LiquidationTile> BankruptcyController::bestLiquidationAsset(Player& player, int amount){
    vector<LiquidationTile> result; 
    int currBalance = player.getBalance();
    if (currBalance >= amount) return result; 

    map<string,vector<StreetTile*>> group;
    for (PropertyTile* p : player.getOwnedProperties()){
        StreetTile* street = dynamic_cast<StreetTile*>(p);
            if (street != NULL){
            if (p->getStatus() == OWNED){
                group[p->getColor()].push_back(street);
            }
        }   
    }
    vector<pair<string,int>> sorted;
    for (map<string, vector<StreetTile*> >::iterator groupIt = group.begin(); groupIt != group.end(); ++groupIt){
        const string& color = groupIt->first;
        vector<StreetTile*>& tiles = groupIt->second;
        int total = 0;
        for (vector<StreetTile*>::iterator tileIt = tiles.begin(); tileIt != tiles.end(); ++tileIt){
            StreetTile* t = *tileIt;
            if (t->getHasHotel()) {
                total += t->getHotelCost()/2;
            }
            total += t->getHouseCount() * (t->getHouseCost()/2);
        }
        sorted.push_back(make_pair(color, total));    
    }

    sort(sorted.begin(), sorted.end(), [](pair<string,int>& a, pair<string, int>& b){
        return a.second > b.second;
    });
    for (auto& [color, _] : sorted){
        auto& tiles = group[color];

        map<StreetTile*, int> houses;
        for (auto* t : tiles) {
            if (t->getHasHotel()) {
                houses[t] = 5; 
            } else {
                houses[t] = t->getHouseCount();
            }
        }
        bool changed = true;
        while (changed && currBalance < amount){
            changed = false;
            int maxHouse = 0;
            for (auto* t : tiles) {
                if (houses[t] > maxHouse) maxHouse = houses[t];
            }
            if (maxHouse > 0){
            for (auto* t : tiles){
                if (houses[t] == maxHouse) { 
                    houses[t]--; 
                    
                    if (maxHouse == 5) {
                        currBalance += t->getHotelCost()/2;
                    } else {
                        currBalance += t->getHouseCost()/2;
                    }
                    
                    LiquidationTile* liquidationTile = new LiquidationTile(t, LiquidationType::SELL);
                    result.push_back(*liquidationTile);
                    changed = true;
                    
                    if (currBalance >= amount) return result;
                    break; 
                }
            }
            }
        }
        for (auto* t : tiles) {
            if (currBalance >= amount) break;
            if (houses[t]==0 && t->getStatus() == OWNED){
                currBalance += t->getMortgageValue();
                LiquidationTile* liquidationTile = new LiquidationTile(t, LiquidationType::MORTGAGE);
                result.push_back(*liquidationTile);
                if(currBalance >= amount) return result;
            }
        }
    }
    for (PropertyTile* p : player.getOwnedProperties()){
        if (!p) continue;

        if (!dynamic_cast<StreetTile*>(p) && p->getStatus() == OWNED){
            currBalance += p->getMortgageValue();
            LiquidationTile* liquidationTile = new LiquidationTile(p, LiquidationType::SELL);
            result.push_back(*liquidationTile);
            if (currBalance >= amount) return result;
        }
    }
    if (currBalance < amount) result.clear();
    return result;
    
}

void BankruptcyController::transferProperties(Player& debitor, Player& creditor){
    vector<PropertyTile*> property = debitor.getOwnedProperties();
    for (PropertyTile* p : property){
        if (!p) continue;
        debitor.removeProperty(p);
        creditor.addProperty(p);
        p->setOwner(&creditor);
    }
}

bool BankruptcyController::checkSolvency(Player& p, int amount){
    if (amount < 0) return true;
    if (p.canAfford(amount)) return true;
    vector<LiquidationTile> result = bestLiquidationAsset(p, amount);
    return !result.empty();
}

vector<LiquidationOption> BankruptcyController::generateOptions(Player& player, const vector<LiquidationOption>& cart, int& initBalance){
    vector<LiquidationOption> options;
    initBalance = player.getBalance();

   map<StreetTile*, int> houses;
    map<PropertyTile*, bool> mortgage;
    
    for (PropertyTile* p : player.getOwnedProperties()) {
        if (!p) continue;
        StreetTile* s = dynamic_cast<StreetTile*>(p);
            if (s != NULL) {
            houses[s] = s->getHasHotel() ? 5 : s->getHouseCount();
        }
        mortgage[p] = (p->getStatus() == MORTGAGED);
    }

    for (vector<LiquidationOption>::const_iterator optIt = cart.begin(); optIt != cart.end(); ++optIt) {
        const LiquidationOption& opt = *optIt;
        if (opt.getType() == LiquidationType::SELL) {
            StreetTile* s = dynamic_cast<StreetTile*>(opt.getTile());
                if (s != NULL) {
                houses[s]--;
            }
        } else if (opt.getType() == LiquidationType::MORTGAGE) {
            mortgage[opt.getTile()] = true;
        }
        initBalance += opt.getValue(); 
    }

    map<string, vector<StreetTile*>> groups;
    for (PropertyTile* p : player.getOwnedProperties()){
        StreetTile* s = dynamic_cast<StreetTile*>(p);
        if (s != NULL){
            if (!mortgage[p] && p->getStatus() == OWNED){
                groups[p->getColor()].push_back(s);
            }
        }
    }

    for (map<string, vector<StreetTile*> >::iterator groupIt = groups.begin(); groupIt != groups.end(); ++groupIt){
        const string& color = groupIt->first;
        vector<StreetTile*>& tiles = groupIt->second;
        bool hasBuilding = false;
        int maxHouse = 0;
        
        for (vector<StreetTile*>::iterator tileIt = tiles.begin(); tileIt != tiles.end(); ++tileIt){
            StreetTile* t = *tileIt;
            if (houses[t] > 0) hasBuilding = true;
            if (houses[t] > maxHouse) maxHouse = houses[t];
        }

        if (hasBuilding){
            for (vector<StreetTile*>::iterator tileIt = tiles.begin(); tileIt != tiles.end(); ++tileIt){
                StreetTile* t = *tileIt;
                if (houses[t] == maxHouse){
                    int val = (maxHouse == 5) ? t->getHotelCost()/2 : t->getHouseCost()/2;
                    LiquidationOption option = LiquidationOption(t, LiquidationType::SELL, val);
                    options.push_back(option);
                }
            }
        } else {
            for (vector<StreetTile*>::iterator tileIt = tiles.begin(); tileIt != tiles.end(); ++tileIt){
                StreetTile* t = *tileIt;
                if (!mortgage[t]){
                    LiquidationOption* option = new LiquidationOption(t, LiquidationType::MORTGAGE, t->getMortgageValue());
                    options.push_back(*option);
                }
            }
        }
    }

    for (PropertyTile* p : player.getOwnedProperties()){
        if (!p) continue;
        if (!dynamic_cast<StreetTile*>(p) && !mortgage[p] && p->getStatus() == OWNED){
            LiquidationOption option = LiquidationOption(p, LiquidationType::MORTGAGE, p->getMortgageValue());
            options.push_back(option);
        }
    }
    
    return options;
}

void BankruptcyController::applyAction(GameContext& ctx, Player& player,LiquidationOption& opt,EconomyController& eco) {
    if (!opt.getTile()) return;
    
    if (opt.getType() == LiquidationType::SELL) {
        StreetTile* street = dynamic_cast<StreetTile*>(opt.getTile());
            if (street != NULL) {
            eco.sellBuilding(player, street);
        }
    }
    else if (opt.getType() == LiquidationType::MORTGAGE) {
            eco.mortgageProperty(player, opt.getTile());
        }
        
}


void BankruptcyController::liquidateAssets(GameContext& ctx,Player& debitor,Player* creditor, int amount,DisplayView& view,EconomyController& eco,InputHandler& input, Tile* bankruptTile){
    vector<LiquidationTile> result = bestLiquidationAsset(debitor, amount);

    if (result.empty()){
        if (bankruptTile == nullptr) {
            this->declareBankruptcy(ctx, debitor, amount, view, eco, input);
        } else {
             if (creditor){
                PropertyTile* tile = dynamic_cast<PropertyTile*>(bankruptTile);
                declareBankruptcy(ctx, debitor, *creditor, amount, view, tile);
            } else {
                TaxTile* taxTile = dynamic_cast<TaxTile*>(bankruptTile);
                declareBankruptcy(ctx, debitor, amount, view, eco, taxTile, input);
            }
        }
        return;
    }

    view.renderBankruptFirstSceneRent(ctx, &debitor, creditor, amount);
    vector<LiquidationOption> cart;
    bool confirmed = false;
    while (!confirmed){
        int initBalance = 0;
        vector<LiquidationOption> options = generateOptions(debitor, cart, initBalance);

        view.liquidatePanel(ctx, &debitor, creditor, amount, options);

        input.getIntInput();
        int choice = input.getIntValue1();

        if (choice == -1) {
            if (initBalance >= amount) {
                confirmed = true;
            } else {
                view.renderInfo("Total liquidation hasn't sufficient funds. Total likuidasi belum mencukupi kewajiban! Please choose another combination of property.\n"); // BIKININ JEJE TOLONG VIEW NYA YA
            }
        } 
        else if (choice == 0) {
            cart.clear();
            view.renderInfo("Keranjang likuidasi dikosongkan.\n");
        } 
        else if (choice >= 1 && choice <= static_cast<int>(options.size())) {
            LiquidationOption selected = options[choice - 1];
            cart.push_back(selected);
            view.renderLiquidateChoose(ctx, &debitor, selected.getTile()); 
        }
    }

    for (vector<LiquidationOption>::iterator optIt = cart.begin(); optIt != cart.end(); ++optIt) {
        applyAction(ctx, debitor, *optIt, eco);
    }

    if (debitor.getBalance() >= amount){
        if (creditor){
            debitor -= amount;
            *creditor += amount;
            view.renderEnoughLiquidate(ctx, &debitor, creditor, amount);
        } else{
            debitor -= amount;
        }
    }
}

void BankruptcyController::declareBankruptcy(GameContext& ctx, Player& player, Player& creditor, int amount, DisplayView& view, PropertyTile* tile) {
    view.renderBankruptSecondScene(ctx, &player, &creditor, tile, amount);

    creditor += player.getBalance();
    player -= player.getBalance();

    transferProperties(player, creditor);

    player.setStatus(BANKRUPT);
}

void BankruptcyController::declareBankruptcy(GameContext& ctx, Player& player, int amount, DisplayView& view, EconomyController& eco, TaxTile* tile, InputHandler& input) {
    view.renderBankruptThirdScene(ctx, &player, tile, amount);

    vector<PropertyTile*> assets = player.getOwnedProperties();

    AuctionController auction;

    for (PropertyTile* prop : assets) {
        prop->setOwner(nullptr); 
        auction.startAuctionBankrupt(ctx, view, input, prop);
    }

    player -= player.getBalance();
    player.setStatus(PlayerStatus::BANKRUPT);

    eco.returnSkillCardsToDeck(player, &ctx);

    player.setStatus(BANKRUPT);
}



// Unused declareBankruptcy, for when the player goes bankrupt to the bank without being on a tax tile
void BankruptcyController::declareBankruptcy(GameContext& ctx, Player& player, int amount, DisplayView& view, EconomyController& eco, InputHandler& input) {
    view.renderBankruptThirdScene(ctx, &player, NULL, amount);

    player -= player.getBalance();
    vector<PropertyTile*> assets = player.getOwnedProperties();

    AuctionController auction;

    for (PropertyTile* prop : assets) {
        prop->setOwner(nullptr); 
        auction.startAuctionBankrupt(ctx, view, input, prop);
    }

    player -= player.getBalance();
    player.setStatus(PlayerStatus::BANKRUPT);

    eco.returnSkillCardsToDeck(player, &ctx);
    
    player.setStatus(BANKRUPT);
}

void BankruptcyController::handleInsufficientFunds(GameContext& ctx, Player& debitor, Player* creditor, int amount, EconomyController& eco, DisplayView& view, InputHandler& input) {
    Tile* currentTile = ctx.getBoard().getTile(ctx.getCurrentPlayer().getPosition());
    if (auto* propertyTile = dynamic_cast<PropertyTile*>(currentTile)) {
        Player* owner = propertyTile->getOwner();
        if (owner != nullptr && owner != &ctx.getCurrentPlayer()) {
            this->declareBankruptcy(ctx, ctx.getCurrentPlayer(), *owner, amount, view, propertyTile);
            return;
        } 
    }

    if (auto* taxTile = dynamic_cast<TaxTile*>(currentTile)) {
        this->declareBankruptcy(ctx, ctx.getCurrentPlayer(), amount, view, eco, taxTile, input);
        return;
    }

    this->declareBankruptcy(ctx, ctx.getCurrentPlayer(), amount, view, eco, input);
};