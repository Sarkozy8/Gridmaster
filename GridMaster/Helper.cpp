#include "Helper.h"

void helper_load_media(const Helper *h, const char *filePath);

bool helper_new(Helper **helper, SDL_Renderer *renderer)
{
    *helper = new Helper;
    if (!*helper)
    {
        std::cerr << "Error in initializing new Helper.\n";
        return false;
    }

    struct Helper *h = *helper;

    h->renderer = renderer;

    return true;
}

void helper_free(Helper **helper)
{
    if (*helper)
    {
        struct Helper *h = *helper;
        h->renderer = nullptr;
        delete h;
        h = nullptr;
        *helper = nullptr;

        std::cout << "Free Helper.\n";
    }
}

// Basic Logic for drawing the helper, actually "helper load media" does the drawing
void helper_draw(const Helper *h, int gamePhase)
{
    switch (gamePhase)
    {
    case 0:
        helper_load_media(h, "Assets/Helper0.png");
        break;
    case 1:
        helper_load_media(h, "Assets/Helper1.png");
        break;
    case 2:
        helper_load_media(h, "Assets/Helper2.png");
        break;
    case 3:
        helper_load_media(h, "Assets/Helper3.png");
        break;
    case 4:
        helper_load_media(h, "Assets/Helper4.png");
        break;
    default:
        if (gamePhase >= 5)
        {
            helper_load_media(h, "Assets/Helper5.png");
        }
        else
        {
            helper_load_media(h, "Assets/Helper0.png");
        }
        break;
    }
}

// Load and draw Helper specified
void helper_load_media(const Helper *h, const char *filePath)
{
    SDL_Surface *imageSurface = IMG_Load(filePath);

    if (!imageSurface)
    {
        std::cerr << "Error loading helper image: " << SDL_GetError() << std::endl;
    }
    else
    {
        SDL_Texture *texture = SDL_CreateTextureFromSurface(h->renderer, imageSurface);
        if (!texture)
        {
            std::cerr << "Error creating texture from surface: " << SDL_GetError() << std::endl;
            SDL_DestroyTexture(texture);
        }
        else
        {
            // Draw the texture to the renderer
            SDL_RenderClear(h->renderer);
            SDL_RenderTexture(h->renderer, texture, nullptr, nullptr);
            SDL_RenderPresent(h->renderer);
            SDL_DestroyTexture(texture);
        }
    }

    SDL_DestroySurface(imageSurface);
}
