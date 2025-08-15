#pragma once
#include "Game.h"

struct Face
{
    SDL_Renderer *renderer = nullptr;
    SDL_Texture *image = nullptr;
    SDL_FRect *src_rects = nullptr;
    SDL_FRect dest_rect;
    unsigned columns;
    unsigned image_index = 0;
};

bool face_new(struct Face **face, SDL_Renderer *renderer, unsigned columns);
void face_free(struct Face **face);
bool face_mouse_click(struct Face *f, float x, float y, bool down);
void face_default(struct Face *f);
void face_won(struct Face *f);
void face_lost(struct Face *f);
void face_question(struct Face *f);
void face_radiation(struct Face *f);
void face_draw(const struct Face *f);
