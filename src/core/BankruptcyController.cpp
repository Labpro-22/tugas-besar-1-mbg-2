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
                if (houses[s] > 0) {
                    houses[s]--;
                } else {
                    mortgage[opt.getTile()] = true;
                }
            } else {
                mortgage[opt.getTile()] = true;
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
                    options.push_back(LiquidationOption(t, LiquidationType::SELL, t->getPrice()));
                    options.push_back(LiquidationOption(t, LiquidationType::MORTGAGE, t->getMortgageValue()));
                }
            }
        }
    }

    for (PropertyTile* p : player.getOwnedProperties()){
        if (!p) continue;
        if (!dynamic_cast<StreetTile*>(p) && !mortgage[p] && p->getStatus() == OWNED){
            options.push_back(LiquidationOption(p, LiquidationType::SELL, p->getPrice()));
            options.push_back(LiquidationOption(p, LiquidationType::MORTGAGE, p->getMortgageValue()));
        }
    }
    
    return options;
}

void BankruptcyController::applyAction(GameContext& ctx, Player& player,LiquidationOption& opt,EconomyController& eco) {
    if (!opt.getTile()) return;
    
    if (opt.getType() == LiquidationType::SELL) {
        StreetTile* street = dynamic_cast<StreetTile*>(opt.getTile());
        if (street != NULL && (street->getHouseCount() > 0 || street->getHasHotel())) {
            eco.sellBuilding(player, street);
        }else {
            player += opt.getValue();
            opt.getTile()->setOwner(nullptr);
            opt.getTile()->setStatus(BANK);
            player.removeProperty(opt.getTile());
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

    vector<string> best = bestLiquidation(debitor, result);
    int total = calculateTotal(debitor,result);

    if (creditor){
        view.renderBankruptFirstSceneRent(ctx, &debitor, creditor, amount, best, total);
    } else {
        TaxTile* taxTile = dynamic_cast<TaxTile*>(bankruptTile);
        view.renderBankruptFirstSceneTax(ctx, &debitor, taxTile,amount, best, total);
    }
    vector<LiquidationOption> cart;
    bool confirmed = false;
    while (!confirmed){
        int initBalance = debitor.getBalance();
        vector<LiquidationOption> options = generateOptions(debitor, cart, initBalance);

        if (options.empty()) {
            view.renderWarning("There's no property left to put in your cart.");
            if (initBalance >= amount) {
                confirmed = true;
            } else {
                view.renderWarning("Total liquidation hasn't paid off the amount of debt.");
                confirmed = true;
            }
            break;
        }

        view.liquidatePanel(ctx, &debitor, creditor, amount, options, initBalance);

        input.getIntInput();
        int choice = input.getIntValue1();

        if (choice == 0) {
            if (initBalance >= amount) {
                confirmed = true;
            } else {
                view.renderPrompt("Total liquidation hasn't paid off the amount of debt! Please choose another combination of property.\n");
            }
        } 
        else if (choice == -1) {
            cart.clear();
            view.renderPrompt("Clearing cart...\n");
        } 
        else if (choice >= 1 && choice <= static_cast<int>(options.size())) {
            LiquidationOption selected = options[choice - 1];
            cart.push_back(selected);
            view.renderLiquidateChoose(ctx, &debitor, selected, initBalance+selected.getValue()); 
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

int BankruptcyController::calculateTotal(Player& player,const vector<LiquidationTile>& best) {
    int total = 0;
    map<StreetTile*, int> temp;
    for (PropertyTile* p : player.getOwnedProperties()) {
        if (StreetTile* s = dynamic_cast<StreetTile*>(p)) {
            temp[s] = s->getHasHotel() ? 5 : s->getHouseCount();
        }
    }
    for (const LiquidationTile& l : best) {
        PropertyTile* pt = l.getTile();
        LiquidationType ltype = l.getType();

        if (ltype == LiquidationType::SELL) {
            StreetTile* st = dynamic_cast<StreetTile*>(pt);
            if (st) {
                if (temp[st] == 5) {
                    total += st->getHotelCost() / 2;
                } else if (temp[st] > 0) {
                    total += st->getHouseCost() / 2;
                } else {
                    total += st->getPrice();
                }
                temp[st]--;
            } else {
                total += pt->getPrice(); 
            }
        } else if (ltype == LiquidationType::MORTGAGE) {
            total += pt->getMortgageValue();
        }
    }
    return total;
}

vector<string> BankruptcyController::bestLiquidation(Player& player, const vector<LiquidationTile>& best) {
    vector<string> result;
    vector<PropertyTile*> tiles;
    vector<LiquidationType> types;
    vector<int> counts;
    vector<int> values;

    map<StreetTile*, int> temp; 
    for (PropertyTile* p : player.getOwnedProperties()) {
        if (StreetTile* s = dynamic_cast<StreetTile*>(p)) {
            temp[s] = s->getHasHotel() ? 5 : s->getHouseCount();
        }
    }

    for (const LiquidationTile& l : best) {
        PropertyTile* pt = l.getTile();
        LiquidationType ltype = l.getType();
        int actionValue = 0;

        if (ltype == LiquidationType::SELL) {
            StreetTile* st = dynamic_cast<StreetTile*>(pt);
            if (st) {
                if (temp[st] == 5) {
                    actionValue = st->getHotelCost() / 2;
                } else if (temp[st] > 0) {
                    actionValue = st->getHouseCost() / 2;
                } else {
                    actionValue = st->getPrice();
                }
                temp[st]--;
            } else {
                actionValue = pt->getPrice();
            }
        } else if (ltype == LiquidationType::MORTGAGE) {
            actionValue = pt->getMortgageValue();
        }

        bool found = false;
        for (int i = 0; i < int(tiles.size()); i++) {
            if (tiles[i] == pt && types[i] == ltype) {
                counts[i]++;
                values[i] += actionValue;
                found = true;
                break;
            }
        }
        
        if (!found) {
            tiles.push_back(pt);
            types.push_back(ltype);
            counts.push_back(1);
            values.push_back(actionValue);
        }
    }

    for (int i = 0; i < int(tiles.size()); i++) {
        if (types[i] == LiquidationType::SELL) {
            StreetTile* st = dynamic_cast<StreetTile*>(tiles[i]);
            if (st != nullptr && (st->getHouseCount() > 0 || st->getHasHotel())) {
                result.push_back("   - SELL " + to_string(counts[i]) + " building(s) on " + st->getName() + " for M" + to_string(values[i]));
            } else {
                result.push_back("   - SELL property " + tiles[i]->getName() + " for M" + to_string(values[i]));
            }
        } else if (types[i] == LiquidationType::MORTGAGE) {
            result.push_back("   - MORTGAGE " + tiles[i]->getName() + " for M" + to_string(values[i]));
        }
    }
    return result;
}