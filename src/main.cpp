#include "SFMLView.hpp"

int main() {
    SFMLView gui; 

    while (gui.isWindowOpen()) {
        gui.processInput();
        gui.renderBoard();
    }

    return 0;
}