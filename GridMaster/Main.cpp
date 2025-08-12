#include "Game.h"

int main()
{
    // Store the game in the Heap memory
    std::unique_ptr<Game> game = std::make_unique<Game>();

    // Initilize the game, if it is successful, run the game
    if (game->Init())
        game->Run();

    return 0;
}
