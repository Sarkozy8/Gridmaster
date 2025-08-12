#pragma once

#include "Game.h"

// Loads media sheet into an SDL_Texture and creates source rectangles for each "frame" in the sheet.

bool load_media_sheet(SDL_Renderer *renderer, SDL_Texture **image, const char *file_path, int width, int height,
                      SDL_FRect **src_rects);
