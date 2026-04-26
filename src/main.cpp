#include <iostream>
#include "views/SFMLView.hpp"
#include "core/GameEngine.hpp"

int main() {
    // 1. Berikan pilihan mode kepada pengguna
    std::cout << "=================================\n";
    std::cout << "      NIMONSPOLI LAUNCHER        \n";
    std::cout << "=================================\n";
    std::cout << "Pilih Mode Tampilan:\n";
    std::cout << "1. Mode Terminal (CLI)\n";
    std::cout << "2. Mode Grafis (GUI SFML)\n";
    std::cout << "Masukkan pilihan (1/2): ";
    
    int choice;
    if (!(std::cin >> choice)) {
        std::cout << "Input tidak valid. Menjalankan mode default (CLI)...\n";
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

    std::cout << "\nTerima kasih telah bermain Nimonspoli!\n";
    return 0;
}