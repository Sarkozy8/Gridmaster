#pragma once
#include "Game.h"

// Display clock for the game

struct Clock
{
    SDL_Renderer *renderer = nullptr;
    SDL_Texture *back_image = nullptr;
    SDL_Texture *digit_image = nullptr;
    SDL_FRect *back_src_rects = nullptr;
    SDL_FRect *digit_src_rects = nullptr;
    SDL_FRect back_dest_rect;
    float digit_width;
    float digit_height;
    unsigned columns;
    unsigned digits[3];
    unsigned seconds;
    Uint64 last_time;
};

bool clock_new(struct Clock **clock, SDL_Renderer *renderer, unsigned columns);
void clock_free(struct Clock **clock);
void clock_reset(struct Clock *c);
void clock_update(struct Clock *c);
void clock_draw(const struct Clock *c);
