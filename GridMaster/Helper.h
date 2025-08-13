#pragma once
#include "Game.h"

// Display Window with Help instrctions
struct Helper
{
    SDL_Renderer *renderer = nullptr;
};

bool helper_new(struct Helper **helper, SDL_Renderer *renderer);
void helper_free(struct Helper **helper);
void helper_draw(const struct Helper *h, int gamePhase);
