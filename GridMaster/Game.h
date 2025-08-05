#pragma once
#include <SDL3/SDL.h>
#include <iostream>

using namespace std;

class Game
{
  public:
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    SDL_Event event;
    bool isRunning = false;
    const int height = 520;
    const int width = 300;

    bool Init()
    {
        window = SDL_CreateWindow("Grid Master", width, height, SDL_WINDOW_ALWAYS_ON_TOP);
        if (!window)
        {
            cerr << "SDL Window didnt create: " << SDL_GetError() << "\n";
            return false;
        }

        renderer = SDL_CreateRenderer(window, NULL);
        if (!renderer)
        {
            cerr << "SDL Renderer didnt create: " << SDL_GetError() << "\n";
            return false;
        }
        isRunning = true;
        return true;
    }

    void Run()
    {
        while (isRunning)
        {
            HandleEvents();
            UpdateState();
            Render();
        }
        Quit();
    }

  private:
    void HandleEvents()
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_EVENT_QUIT:
                isRunning = false;
                break;
            case SDL_EVENT_KEY_DOWN:
                if (event.key.key == SDLK_ESCAPE)
                {
                    isRunning = false;
                }
            }
        }
    }
    void UpdateState()
    {
    }
    void Render()
    {
    }
    void Quit()
    {
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
    }
};
