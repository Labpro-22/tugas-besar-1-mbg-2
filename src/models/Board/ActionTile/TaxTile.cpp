#include "TaxTile.hpp"

TaxTile::TaxTile(int idx, string code, string name, bool isPPH, string color) 
    : ActionTile(idx, code, name, TAX_TILE, color), isPPH(isPPH){

}
LandResult TaxTile::land(GameContext &G){
    return LandResult{LandEventType::PAYTAX, this, nullptr, &G.getCurrentPlayer(), nullptr, 0, false};
};

bool TaxTile::getIsPPH() const {
    return isPPH;
}
    