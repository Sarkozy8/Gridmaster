
#include "Game.h"

int main()
{
    std::unique_ptr<Game> game = std::make_unique<Game>();

    if (game->Init())
        game->Run();

    return 0;
}
