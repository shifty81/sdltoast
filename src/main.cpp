#include "engine/Game.h"
#include <iostream>
#include <memory>

int main(int argc, char* argv[]) {
    std::cout << "==================================" << std::endl;
    std::cout << "   Harvest Quest - Alpha Build   " << std::endl;
    std::cout << "==================================" << std::endl;
    std::cout << "A game combining Zelda: A Link to the Past" << std::endl;
    std::cout << "and Stardew Valley mechanics!" << std::endl;
    std::cout << std::endl;
    std::cout << "Controls:" << std::endl;
    std::cout << "  WASD / Arrow Keys - Move" << std::endl;
    std::cout << "  ESC - Quit" << std::endl;
    std::cout << "==================================" << std::endl;
    std::cout << std::endl;

    auto game = std::make_unique<Game>();

    if (!game->Initialize("Harvest Quest - Zelda meets Stardew Valley", 800, 600)) {
        std::cerr << "Failed to initialize game!" << std::endl;
        return 1;
    }

    game->Run();
    game->Shutdown();

    std::cout << "Thanks for playing!" << std::endl;
    return 0;
}
