#pragma once
#include "Game.h"

// Display border for the game

struct Border
{
    SDL_Renderer *renderer = nullptr;
    SDL_Texture *image = nullptr;
    SDL_FRect *src_rects = nullptr;
    unsigned rows;
    unsigned columns;
    float piece_height;
    float piece_width;
    float left_offset;
};

bool border_new(struct Border **border, SDL_Renderer *renderer, unsigned rows, unsigned columns);
void border_free(struct Border **border);
void border_draw(const struct Border *b);
