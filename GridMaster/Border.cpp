#include "Border.h"
#include "Load_Sprite_Sheet.h"

bool Border_Init(Border **border, SDL_Renderer *renderer, unsigned int rows, unsigned int columns, float tileWidth,
                 float tileHeight, float offsetX)
{
    border->renderer = renderer;
    border->rows = rows;
    border->columns = columns;

    return true;
}

void Border_Render(Border &border)
{
}

void Border_Clear(Border &border)
{
}
