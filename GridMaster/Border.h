#pragma once
#include "Game.h"

struct Border
{
    SDL_Renderer *renderer = nullptr;
    SDL_Texture *texture = nullptr;
    SDL_FRect *srcRects = nullptr;
    unsigned int rows;
    unsigned int columns;
    float tileWidth;
    float tileHeight;
    float offsetX;
};

bool Border_Init(Border **border, SDL_Renderer *renderer, unsigned int rows, unsigned int columns, float tileWidth,
                 float tileHeight, float offsetX);

void Border_Render(Border &border);
void Border_Clear(Border &border);
