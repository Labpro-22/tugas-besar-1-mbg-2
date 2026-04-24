#include "BankruptcyController.hpp"

vector<LiquidationTile> BankruptcyController::bestLiquidationAsset(Player& player, int amount){
    vector<LiquidationTile> result; 
    int currBalance = player.getBalance();
    if (currBalance >= amount) return result; 

    map<string,vector<StreetTile*>> group;
    for (PropertyTile* p : player.getOwnedProperties()){
        if (auto* street = dynamic_cast<StreetTile*>(p)){
            if (p->getStatus() == OWNED){
                group[p->getColor()].push_back(street);
            }
        }   
    }
    vector<pair<string,int>> sorted;
    for (auto& [color,tiles] : group){
        int total = 0;
        for (auto* t : tiles){
            if (t->getHasHotel()) {
                total += t->getHotelCost()/2;
            }
            total += t->getHouseCount() * (t->getHouseCost()/2);
        }
        sorted.push_back({color, total});    
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
                    
                    result.push_back({t, LiquidationType::SELL});
                    changed = true;
                    
                    if (currBalance >= amount) return result;
                    break; 
                }
            }
            }
        }
        for (auto* t: tiles){
            if (currBalance >= amount) break;
            if (houses[t]==0 && t->getStatus() == OWNED){
                currBalance += t->getMortgageValue();
                result.push_back({t,LiquidationType::MORTGAGE});
                if(currBalance >= amount) return result;
            }
        }
    }
    for (PropertyTile* p : player.getOwnedProperties()){
        if (!p) continue;

        if (!dynamic_cast<StreetTile*>(p) && p->getStatus() == OWNED){
            currBalance += p->getMortgageValue();
            result.push_back({p, LiquidationType::SELL});
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

vector<LiquidationOption> BankruptcyController::generateOptions(Player& player){
    vector<LiquidationOption> options;

    map<string, vector<StreetTile*>> groups;

    for (PropertyTile* p : player.getOwnedProperties()){
        if (!p) continue;

        if (auto* s = dynamic_cast<StreetTile*>(p)){
            if (p->getStatus() == OWNED){
                groups[p->getColor()].push_back(s);
            }
        }
    }

    for (auto& [color, tiles] : groups){

        bool hasBuilding = false;

        for (auto* t : tiles){
            if (t->getHouseCount() > 0 || t->getHasHotel()){
                hasBuilding = true;
                break;
            }
        }

        for (auto* t : tiles){
            if (t->getHasHotel()){
                options.push_back({t,LiquidationType::SELL,t->getHotelCost()/2});
            }
            else if (t->getHouseCount() > 0){
                options.push_back({t,LiquidationType::SELL,t->getHouseCost()/2});
            }
        }

        if (!hasBuilding){
            for (auto* t : tiles){
                options.push_back({t,LiquidationType::MORTGAGE,t->getMortgageValue()});
            }
        }
    }

    for (PropertyTile* p : player.getOwnedProperties()){
        if (!p) continue;

        if (!dynamic_cast<StreetTile*>(p) && p->getStatus() == OWNED){
            options.push_back({p,LiquidationType::MORTGAGE,p->getMortgageValue()});
        }
    }
    return options;
}

void BankruptcyController::applyAction(GameContext& ctx, Player& player,LiquidationOption& opt,EconomyController& eco) {
    if (!opt.tile) return;
    
    if (opt.type == LiquidationType::SELL) {
        if (auto* street = dynamic_cast<StreetTile*>(opt.tile)) {
            eco.sellBuilding(player, street);
        }
    }
    else if (opt.type == LiquidationType::MORTGAGE) {
            eco.mortgageProperty(player, opt.tile);
        }
        
}


void BankruptcyController::liquidateAssets(GameContext& ctx,Player& debitor,Player* creditor,int amount,DisplayView& view,EconomyController& eco,InputHandler& input,PropertyTile* bankruptTile,TaxTile* taxTile){
    vector<LiquidationTile> result = bestLiquidationAsset(debitor, amount);

    if (result.empty()){
        if (creditor){
            declareBankruptcy(ctx, debitor, *creditor, amount, view, bankruptTile);
        } else{
            declareBankruptcy(ctx, debitor, amount, view, eco, taxTile);
        }
        return;
    }

    while (debitor.getBalance() < amount){

        vector<LiquidationOption> options = generateOptions(debitor);

        if (options.empty()){
            if (creditor){
                declareBankruptcy(ctx, debitor, *creditor, amount, view, bankruptTile);
            } else{
                declareBankruptcy(ctx, debitor, amount, view, eco, taxTile);
            }
            return;
        }

        view.renderBankruptFirstSceneRent(ctx, &debitor, creditor, amount);
        view.liquidatePanel(ctx, &debitor, creditor, amount, options);

        input.getIntInput();
        int choice = input.getIntValue1();

        if (choice == 0) break;
        if (choice < 1 || choice > int(options.size())) continue;

        LiquidationOption selected = options[choice - 1];

        applyAction(ctx,debitor, selected, eco);

        view.renderLiquidateChoose(ctx, &debitor, selected.tile);

        if (debitor.getBalance() < amount){
            view.renderNotEnoughLiquidate(ctx, &debitor, amount);
        }
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

void BankruptcyController::declareBankruptcy(GameContext& ctx, Player& player, int amount, DisplayView& view, EconomyController& eco, TaxTile* tile) {
    view.renderBankruptThirdScene(ctx, &player, tile, amount);

    player -= player.getBalance();

    eco.returnAllAssetsToBank(player);

    player.setStatus(BANKRUPT);
}

