#include "load_media.h"

// Note: The size for the rectangles is determines by width and height parameters.

bool load_media_sheet(SDL_Renderer *renderer, SDL_Texture **image, const char *file_path, int width, int height,
                      SDL_FRect **src_rects)
{
    if (*image)
    {
        SDL_DestroyTexture(*image);
        *image = nullptr;
    }
    if (*src_rects)
    {
        delete[] *src_rects;
        *src_rects = nullptr;
    }

    *image = IMG_LoadTexture(renderer, file_path);
    if (!*image)
    {
        std::cerr << "Error loading Texture: " << SDL_GetError() << std::endl;
        return false;
    }

    int rows = (*image)->h / height;
    int columns = (*image)->w / width;

    if (!SDL_SetTextureScaleMode(*image, SDL_SCALEMODE_NEAREST))
    {
        std::cerr << "Error setting Texture scale mode: " << SDL_GetError() << std::endl;
        return false;
    }

    *src_rects = new SDL_FRect[rows * columns]();
    if (*src_rects == nullptr)
    {
        std::cerr << "Error allocating src_rects." << std::endl;
        return false;
    }

    for (int row = 0; row < rows; row++)
    {
        for (int column = 0; column < columns; column++)
        {
            int index = row * columns + column;
            (*src_rects)[index].x = static_cast<float>(column * width);
            (*src_rects)[index].y = static_cast<float>(row * height);
            (*src_rects)[index].w = static_cast<float>(width);
            (*src_rects)[index].h = static_cast<float>(height);
        }
    }

    return true;
}
