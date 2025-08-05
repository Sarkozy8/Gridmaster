#pragma once
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <memory>
#include <iostream>

class Game
{
  public:
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    SDL_Event event;
    bool isRunning = false;
    const int height = 700;
    const int width = 500;

    Uint64 lastFrameTime = 0;

    bool Init();
    void Run();

  private:
    void HandleEvents();
    void UpdateState();
    void Render();
    void Quit();
};
