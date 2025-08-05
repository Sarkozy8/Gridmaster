#pragma once

#include "Game.h"

bool Load_Sprite_Sheet(SDL_Renderer *renderer, SDL_Texture **texture, const char *filePath, int width, int height,
                       SDL_FRect **firstRect);
