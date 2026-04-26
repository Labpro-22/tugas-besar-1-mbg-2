#include <iostream>
#include "views/SFMLView.hpp"
#include "core/GameEngine.hpp"

int main() {
    std::cout << "=================================\n";
    std::cout << "      NIMONSPOLI LAUNCHER        \n";
    std::cout << "=================================\n";
    std::cout << "Select Display Mode:\n";
    std::cout << "1. Terminal Mode (CLI)\n";
    std::cout << "2. Graphical Mode (GUI SFML)\n";
    std::cout << "Enter choice (1/2): ";
    
    int choice;
    if (!(std::cin >> choice)) {
        std::cout << "Invalid input. Running default mode (CLI)...\n";
        choice = 1;
    }

    if (choice == 2) {
        SFMLView gui;
        GameEngine game(&gui);

        game.run();
    } 
    else {
        
        GameEngine game;
        game.run();
    }

    std::cout << "\nThank you for playing Nimonspoli!\n";
    return 0;
}