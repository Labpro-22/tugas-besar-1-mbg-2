#include "TaxTile.hpp"

TaxTile::TaxTile(int idx, string code, string name, bool isPPH) 
    : ActionTile(idx, code, name, TAX_TILE), isPPH(isPPH) {

}
LandResult TaxTile::land(GameContext &G){
    return LandResult{LandEventType::PAYTAX, this, nullptr, G.getCurrentPlayer(), nullptr, 0, false, string("You landed on a tax tile. You need to pay ") + (isPPH ? "PPh" : "PBM") + string(" of M") + to_string(isPPH ? 200 : 100) + string(".")};
};

