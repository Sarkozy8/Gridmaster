#include "Load_Sprite_Sheet.h"

bool Load_Sprite_Sheet(SDL_Renderer *renderer, SDL_Texture **texture, const char *filePath, int width, int height,
                       SDL_FRect **firstRects)
{
    if (*texture)
    {
        SDL_DestroyTexture(*texture);
        *texture = nullptr;
    }
    if (firstRects)
    {
        delete firstRects;
        firstRects = nullptr;
    }

    *texture = IMG_LoadTexture(renderer, filePath);
    if (!*texture)
    {
        std::cerr << "Failed to load texture: " << SDL_GetError() << "\n";
        return false;
    }

    int rows = (*texture)->h / height;
    int columns = (*texture)->w / width;

    if (!SDL_SetTextureScaleMode(*texture, SDL_ScaleMode::SDL_SCALEMODE_NEAREST))
    {
        std::cerr << "Failed to set texture scale mode: " << SDL_GetError() << "\n";
        SDL_DestroyTexture(*texture);
        *texture = nullptr;
        return false;
    }

    *firstRects = new SDL_FRect[columns * rows];

    if (!*firstRects)
    {
        std::cerr << "Failed to allocate memory for rectangles\n";
        SDL_DestroyTexture(*texture);
        *texture = nullptr;
        return false;
    }

    for (int row = 0; row < rows; row++)
    {
        for (int column = 0; column < columns; columns++)
        {
            int index = row * columns + column;
            (*firstRects)[index].x = column * width;
            (*firstRects)[index].y = row * height;
            (*firstRects)[index].w = width;
            (*firstRects)[index].h = height;
        }
    }

    return true;
}
