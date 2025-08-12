#pragma once
#include "Game.h"

// Mainly used for the digit display of mines left

struct Mines
{
    SDL_Renderer *renderer = nullptr;
    SDL_Texture *back_image = nullptr;
    SDL_Texture *digit_image = nullptr;
    SDL_FRect *back_src_rects = nullptr;
    SDL_FRect *digit_src_rects = nullptr;
    SDL_FRect back_dest_rect;
    float digit_width;
    float digit_height;
    unsigned digits[3];
    int mine_count;
};

bool mines_new(struct Mines **mines, SDL_Renderer *renderer, int mine_count);
void mines_free(struct Mines **mines);
void mines_reset(struct Mines *m, int mine_count);
void mines_increment(struct Mines *m);
void mines_decrement(struct Mines *m);
void mines_draw(const struct Mines *m);
