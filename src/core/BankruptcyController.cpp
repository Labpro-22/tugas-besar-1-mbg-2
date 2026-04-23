#include "BankruptcyController.hpp"

int BankruptcyController::calculatePropertySell(PropertyTile* property){
    if(property == nullptr || property->getStatus() == MORTGAGED) {
        return 0;
    }

    int total = property->getPrice();
    if (auto* street = dynamic_cast<StreetTile*>(property)){
        total += street->getBuildingValue()/2;
    }
    return total;
} 

int BankruptcyController::calculateMaxLiquidation(Player& player){
    int max = accumulate(player.getOwnedProperties().begin(),player.getOwnedProperties().end(),0,[](int acc, const PropertyTile& p){
        if (p.getPropertyType() == MORTGAGED) return acc + p.getMortgageValue();
        else return acc+ p.getPrice();
    });
    max += player.getBalance();
    return max;
}

void BankruptcyController::transferProperties(Player& debitor, Player& creditor){
    vector<PropertyTile*> property = debitor.getOwnedProperties();
    for (PropertyTile* p : property){
        if (p == nullptr) continue;
        debitor.removeProperty(p);
        creditor.addProperty(p);
        p->setOwner(&creditor);
    }
}

bool BankruptcyController::checkSolvency(Player& p, int amount){
    if (amount <= 0 ) return true;
    if (p.canAfford(amount)) return true;
    return calculateMaxLiquidation(p) >= amount;
}

bool BankruptcyController::liquidateAssets()