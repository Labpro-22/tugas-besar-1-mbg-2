#include "SFMLView.hpp"
#include "../include/core/GameEngine.hpp"

int main() {
    SFMLView gui; 

    while (gui.isWindowOpen()) {
        gui.processInput();
        gui.renderBoard();
    }

    GameEngine game;
    game.run();

    return 0;
}
