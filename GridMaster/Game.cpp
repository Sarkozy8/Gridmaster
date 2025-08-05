#include "Game.h"

bool Game::Init()
{
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
    {
        std::cerr << "SDL didnt initialize: " << SDL_GetError() << "\n";
        return false;
    }

    window = SDL_CreateWindow("Grid Master", width, height, 0);
    if (!window)
    {
        std::cerr << "SDL Window didnt create: " << SDL_GetError() << "\n";
        return false;
    }

    renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer)
    {
        std::cerr << "SDL Renderer didnt create: " << SDL_GetError() << "\n";
        return false;
    }

    SDL_Surface *iconSurface = IMG_Load("assets/bombIcon.png");
    if (iconSurface)
    {
        if (!SDL_SetWindowIcon(window, iconSurface))
        {
            std::cerr << "Failed to set window icon: " << SDL_GetError() << "\n";
            SDL_DestroySurface(iconSurface);
            return false;
        }
    }
    else
    {
        std::cerr << "Failed to load icon surface: " << SDL_GetError() << "\n";
        SDL_DestroySurface(iconSurface);
        return false;
    }

    // Clean up the icon surface after setting the icon
    SDL_DestroySurface(iconSurface);

    // Start Gane Loop
    isRunning = true;

    return true;
}

void Game::Run()
{
    while (isRunning)
    {
        HandleEvents();
        UpdateState();
        Render();
    }
    Quit();
}

void Game::HandleEvents()
{
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_EVENT_QUIT:
            isRunning = false;
            break;
        case SDL_EVENT_KEY_DOWN:
            switch (event.key.scancode)
            {
            case SDL_SCANCODE_ESCAPE:
                isRunning = false; // Exit on Escape key
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
    }
}

void Game::UpdateState()
{
    // Calculate delta time
    Uint64 deltaTime = SDL_GetTicks() - lastFrameTime;

    // Limit the frame rate to 60 FPS
    if (deltaTime < 16) // Reason for 16: 1000ms / 60fps = ~16ms per frame
    {
        SDL_Delay(16 - deltaTime);
    }

    lastFrameTime = SDL_GetTicks(); // Update last frame time
}

void Game::Render()
{
    SDL_RenderClear(renderer); // Clears the renderer (And set background color to the last drawn color)
    // Draw Everything here
    SDL_RenderPresent(renderer); // Swap buffers
}

void Game::Quit()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}
