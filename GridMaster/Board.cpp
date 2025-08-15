#include "Board.h"

void board_free_arrays(struct Board *b);
bool board_allocate_arrays(struct Board *b);
bool board_uncover(struct Board *b);
bool board_push_check(struct Board *b, int row, int column);
struct Node board_pop_check(struct Board *b);
void board_reveal(struct Board *b);
void board_check_won(struct Board *b);
void board_assign_special_mines_arrays(struct Board *b, int type, int row, int column);

// Initializes a new board
bool board_new(struct Board **board, SDL_Renderer *renderer, unsigned rows, unsigned columns, int mine_count)
{
    *board = new Board;
    if (!*board)
    {
        std::cerr << "Error in initializing new board.\n";
        return false;
    }
    Board *b = *board;

    b->renderer = renderer;
    b->rows = rows;
    b->columns = columns;
    b->mine_count = mine_count;

    b->piece_size = PIECE_SIZE * 2;
    b->rect.x = (PIECE_SIZE - BORDER_LEFT) * 2;
    b->rect.y = BORDER_HEIGHT * 2;
    b->rect.w = (float)b->columns * b->piece_size;
    b->rect.h = (float)b->rows * b->piece_size;

    if (!load_media_sheet(b->renderer, &b->image, "Assets/boardFinal.png", PIECE_SIZE, PIECE_SIZE, &b->src_rects))
    {
        return false;
    }

    if (!board_reset(b, b->mine_count, true, b->gamePhase))
    {
        return false;
    }

    return true;
}

// Frees the board arrays and textures, used in board_free
void board_free_arrays(struct Board *b)
{
    if (b->front_array)
    {
        for (unsigned row = 0; row < b->rows; row++)
        {
            if (b->front_array[row])
            {
                delete[] b->front_array[row];
                b->front_array[row] = nullptr;
            }
        }
        delete[] b->front_array;
        b->front_array = nullptr;
    }

    if (b->back_array)
    {
        for (unsigned row = 0; row < b->rows; row++)
        {
            if (b->back_array[row])
            {
                delete[] b->back_array[row];
                b->back_array[row] = nullptr;
            }
        }
        delete[] b->back_array;
        b->back_array = nullptr;
    }

    if (b->nuclear_array)
    {
        for (unsigned row = 0; row < b->rows; row++)
        {
            if (b->nuclear_array[row])
            {
                delete[] b->nuclear_array[row];
                b->nuclear_array[row] = nullptr;
            }
        }
        delete[] b->nuclear_array;
        b->nuclear_array = nullptr;
    }

    if (b->proximity_array)
    {
        for (unsigned row = 0; row < b->rows; row++)
        {
            if (b->proximity_array[row])
            {
                delete[] b->proximity_array[row];
                b->proximity_array[row] = nullptr;
            }
        }
        delete[] b->proximity_array;
        b->proximity_array = nullptr;
    }
}

// Clean up the board
void board_free(struct Board **board)
{
    if (*board)
    {
        struct Board *b = *board;

        board_free_arrays(b);

        if (b->src_rects)
        {
            delete[] b->src_rects;
            b->src_rects = nullptr;
        }

        if (b->image)
        {
            SDL_DestroyTexture(b->image);
            b->image = nullptr;
        }

        b->renderer = nullptr;

        delete b;
        b = nullptr;
        *board = nullptr;

        std::cout << "Board clean.\n";
    }
}

bool board_allocate_arrays(struct Board *b)
{
    b->front_array = new (std::nothrow) unsigned *[b->rows];
    if (!b->front_array)
    {
        std::cerr << "Error in Allocating front array of rows.\n";
        return false;
    }

    for (unsigned row = 0; row < b->rows; row++)
    {
        b->front_array[row] = new (std::nothrow) unsigned[b->columns]();
        if (b->front_array[row] == NULL)
        {
            std::cerr << "Error in allocating front array of columns.\n";
            return false;
        }
    }

    b->back_array = new (std::nothrow) unsigned *[b->rows];
    if (!b->back_array)
    {
        std::cerr << "Error in Allocating back array of rows.\n";
        return false;
    }

    for (unsigned row = 0; row < b->rows; row++)
    {
        b->back_array[row] = new (std::nothrow) unsigned[b->columns]();
        if (b->back_array[row] == NULL)
        {
            std::cerr << "Error in allocating back array of columns.\n";
            return false;
        }
    }

    b->nuclear_array = new (std::nothrow) unsigned *[b->rows];
    if (!b->nuclear_array)
    {
        std::cerr << "Error in Allocating nuclear array of rows.\n";
        return false;
    }

    for (unsigned row = 0; row < b->rows; row++)
    {
        b->nuclear_array[row] = new (std::nothrow) unsigned[b->columns]();
        if (b->nuclear_array[row] == NULL)
        {
            std::cerr << "Error in allocating nuclear array of columns.\n";
            return false;
        }
    }

    b->proximity_array = new (std::nothrow) unsigned *[b->rows];
    if (!b->proximity_array)
    {
        std::cerr << "Error in Allocating proximity array of rows.\n";
        return false;
    }
    for (unsigned row = 0; row < b->rows; row++)
    {
        b->proximity_array[row] = new (std::nothrow) unsigned[b->columns]();
        if (b->proximity_array[row] == NULL)
        {
            std::cerr << "Error in allocating proximity array of columns.\n";
            return false;
        }
    }

    return true;
}

bool board_reset(struct Board *b, int mine_count, bool full_reset, int gamePhase)
{
    b->mine_count = mine_count;
    b->gamePhase = gamePhase;

    // Resets the board, either fully or partially
    if (full_reset)
    {
        board_free_arrays(b);

        if (!board_allocate_arrays(b))
        {
            return false;
        }

        for (unsigned row = 0; row < b->rows; row++)
        {
            for (unsigned column = 0; column < b->columns; column++)
            {
                b->front_array[row][column] = 9;
            }
        }
    }
    else
    {
        for (unsigned row = 0; row < b->rows; row++)
        {
            for (unsigned column = 0; column < b->columns; column++)
            {
                b->back_array[row][column] = 0;
                unsigned elem = b->front_array[row][column];
                b->front_array[row][column] = (elem == 10 || elem == 11) ? elem : 9;
            }
        }
    }

    // Clear the nuclear and proximity arrays in case of a partial reset
    for (unsigned row = 0; row < b->rows; row++)
    {
        for (unsigned column = 0; column < b->columns; column++)
        {
            b->nuclear_array[row][column] = 0;
            b->proximity_array[row][column] = 0;
        }
    }

    // Add mines to the back array
    int add_mines = b->mine_count;
    while (add_mines > 0)
    {
        int r = rand() % (int)b->rows;
        int c = rand() % (int)b->columns;
        if (!b->back_array[r][c])
        {
            int mine_type = 1 + rand() % 3;
            if (mine_type == 2 && b->gamePhase > 0)
            {
                b->nuclear_array[r][c] = 2; // Proximity mine
                board_assign_special_mines_arrays(b, mine_type, r, c);
            }
            else if (mine_type == 3 && b->gamePhase > 2)
            {
                b->proximity_array[r][c] = 2; // Nuclear mine
                board_assign_special_mines_arrays(b, mine_type, r, c);
            }

            b->back_array[r][c] = 13;
            add_mines--;
        }
    }

    // Count the mines around each cell and assigned to the array
    for (int row = 0; row < (int)b->rows; row++)
    {
        for (int column = 0; column < (int)b->columns; column++)
        {
            unsigned close_mines = 0;
            if (b->back_array[row][column] == 13)
            {
                continue;
            }
            for (int r = row - 1; r < row + 2; r++)
            {
                if (r >= 0 && r < (int)b->rows)
                {
                    for (int c = column - 1; c < column + 2; c++)
                    {
                        if (c >= 0 && c < (int)b->columns)
                        {
                            if (b->back_array[r][c] == 13)
                            {
                                close_mines++;
                            }
                        }
                    }
                }
            }
            b->back_array[row][column] = close_mines;
        }
    }

    b->first_turn = true;
    b->game_state = GAME_PLAY;

    return true;
}

enum GameState board_game_state(const struct Board *b)
{
    return b->game_state;
}

int board_mine_marked(const struct Board *b)
{
    return b->mine_marked;
}

bool board_is_pressed(const struct Board *b)
{
    return b->left_pressed || b->right_pressed;
}

bool board_uncover(struct Board *b)
{

    // Uncover the cells around the clicked cell if it is empty
    while (b->check_head)
    {
        struct Node pos = board_pop_check(b);

        for (int r = pos.row - 1; r < pos.row + 2; r++)
        {
            if (r < 0 || r >= (int)b->rows)
            {
                continue;
            }
            for (int c = pos.column - 1; c < pos.column + 2; c++)
            {
                if (c < 0 || c >= (int)b->columns)
                {
                    continue;
                }
                if (b->front_array[r][c] == 9)
                {
                    b->front_array[r][c] = b->back_array[r][c];
                    if (b->front_array[r][c] == 0)
                    {
                        if (!board_push_check(b, r, c))
                        {
                            return false;
                        }
                    }
                }
            }
        }
    }

    return true;
}

bool board_push_check(struct Board *b, int row, int column)
{
    struct Node *node = new Node;
    if (!node)
    {
        std::cerr << "Error allocating Check Node." << std::endl;
        return false;
    }

    node->row = row;
    node->column = column;

    node->next = b->check_head;
    b->check_head = node;

    return true;
}

struct Node board_pop_check(struct Board *b)
{
    struct Node pos = {0};
    if (b->check_head)
    {
        pos.row = b->check_head->row;
        pos.column = b->check_head->column;

        struct Node *node = b->check_head;
        b->check_head = b->check_head->next;
        delete node;
    }

    return pos;
}

void board_reveal(struct Board *b)
{
    for (unsigned row = 0; row < b->rows; row++)
    {
        for (unsigned column = 0; column < b->columns; column++)
        {
            if ((b->front_array[row][column] == 9 || b->front_array[row][column] == 11) &&
                b->back_array[row][column] == 13)
            {
                b->front_array[row][column] = 13;
            }
            if ((b->front_array[row][column] == 10 || b->front_array[row][column] == 12) &&
                b->back_array[row][column] != 13)
            {
                b->front_array[row][column] = 15;
            }
        }
    }
}

void board_check_won(struct Board *b)
{
    for (unsigned row = 0; row < b->rows; row++)
    {
        for (unsigned column = 0; column < b->columns; column++)
        {
            if (b->back_array[row][column] != 13)
            {
                if (b->back_array[row][column] != b->front_array[row][column])
                {
                    return;
                }
            }
        }
    }

    b->game_state = GAME_WON;
}

bool IsHoveringNuclear(struct Board *b, float x, float y, int &outRow, int &outCol)
{
    if (x >= b->rect.x && x < b->rect.x + b->rect.w && y >= b->rect.y && y < b->rect.y + b->rect.h)
    {
        int row = (int)((y - b->rect.y) / b->piece_size);
        int col = (int)((x - b->rect.x) / b->piece_size);
        if (b->nuclear_array[row][col] > 0)
        {
            outRow = row;
            outCol = col;
            std::cout << "Hovering near a nuclear mine at (" << row << ", " << col << ")\n";
            return true;
        }
    }
    return false;
}

bool IsHoveringProximity(struct Board *b, float x, float y, int &outRow, int &outCol)
{
    if (x >= b->rect.x && x < b->rect.x + b->rect.w && y >= b->rect.y && y < b->rect.y + b->rect.h)
    {
        int row = (int)((y - b->rect.y) / b->piece_size);
        int col = (int)((x - b->rect.x) / b->piece_size);
        if (b->proximity_array[row][col] == 1)
        {
            outRow = row;
            outCol = col;
            std::cout << "Hovering near a proximity mine at (" << row << ", " << col << ")\n";
            return true;
        }
    }
    return false;
}

bool HoverOverProximity(struct Board *b, float x, float y, int &outRow, int &outCol)
{
    if (x >= b->rect.x && x < b->rect.x + b->rect.w && y >= b->rect.y && y < b->rect.y + b->rect.h)
    {
        int row = (int)((y - b->rect.y) / b->piece_size);
        int col = (int)((x - b->rect.x) / b->piece_size);
        if (b->proximity_array[row][col] == 2)
        {
            outRow = row;
            outCol = col;
            std::cout << "Hover over a proximity mine at (" << row << ", " << col << ")\n";
            return true;
        }
    }
    return false;
}

void board_mouse_down(struct Board *b, float x, float y, Uint8 button)
{
    if (button == SDL_BUTTON_LEFT)
    {
        b->left_pressed = false;
    }
    else if (button == SDL_BUTTON_RIGHT)
    {
        b->right_pressed = false;
    }

    // Detects if the mouse is within the board rectangle and which piece was clicked
    if (x >= b->rect.x && x < b->rect.x + b->rect.w)
    {
        if (y >= b->rect.y && y < b->rect.y + b->rect.h)
        {
            int row = (int)((y - b->rect.y) / b->piece_size);
            int column = (int)((x - b->rect.x) / b->piece_size);
            if (button == SDL_BUTTON_LEFT)
            {
                if (b->front_array[row][column] == 9)
                {
                    b->left_pressed = true;
                }
            }
            else if (button == SDL_BUTTON_RIGHT)
            {
                if (b->front_array[row][column] > 8 && b->front_array[row][column] <= 12)
                {
                    b->right_pressed = true;
                }
            }
        }
    }
}

bool board_mouse_up(struct Board *b, float x, float y, Uint8 button, const int &gamePhase)
{
    b->mine_marked = 0;

    if (button == SDL_BUTTON_LEFT)
    {
        if (!b->left_pressed)
        {
            return true;
        }
        else
        {
            b->left_pressed = false;
        }
    }
    else if (button == SDL_BUTTON_RIGHT)
    {
        if (!b->right_pressed)
        {
            return true;
        }
        else
        {
            b->right_pressed = false;
        }
    }

    if (x < b->rect.x || x >= b->rect.x + b->rect.w)
    {
        return true;
    }
    if (y < b->rect.y || y >= b->rect.y + b->rect.h)
    {
        return true;
    }

    int row = (int)((y - b->rect.y) / b->piece_size);
    int column = (int)((x - b->rect.x) / b->piece_size);

    if (button == SDL_BUTTON_LEFT)
    {
        if (b->front_array[row][column] == 9)
        {
            while (true)
            {
                if (b->back_array[row][column] == 13)
                {
                    b->front_array[row][column] = 14;
                    b->game_state = GAME_LOST;
                }
                else
                {
                    b->front_array[row][column] = b->back_array[row][column];
                    if (b->back_array[row][column] == 0)
                    {
                        if (!board_push_check(b, row, column))
                        {
                            return false;
                        }
                        if (!board_uncover(b))
                        {
                            return false;
                        }
                    }
                    board_check_won(b);
                }
                if (b->first_turn && b->game_state != GAME_PLAY)
                {
                    if (!board_reset(b, b->mine_count, false, b->gamePhase))
                    {
                        return false;
                    }
                }
                else
                {
                    break;
                }
            }
            b->first_turn = false;

            if (b->game_state != GAME_PLAY)
            {
                board_reveal(b);
            }
        }
    }

    // Cycles through the mine marking states
    if (button == SDL_BUTTON_RIGHT)
    {
        // BUG: Doesnt keep cycling after a cell is number 12 (nuclear)
        std::cout << "Right Clicked on (" << row << ", " << column << ")\n";
        if (b->front_array[row][column] == 9)
        {
            b->front_array[row][column]++;
            b->mine_marked = -1;
        }
        else if (b->front_array[row][column] == 10)
        {
            b->front_array[row][column]++;
            b->mine_marked = 1;
        }
        else if (b->front_array[row][column] == 11)
        {
            if (gamePhase > 0)
            {
                b->front_array[row][column]++;
                b->mine_marked = -1;
            }
            else
            {
                b->front_array[row][column] = 9;
            }
        }
        else if (b->front_array[row][column] == 12)
        {
            b->front_array[row][column] = 9;
            b->mine_marked = 1;
        }
    }

    return true;
}

// Assigns the interactible positions around a mine for the nuclear or proximity mines
void board_assign_special_mines_arrays(struct Board *b, int type, int row, int column)
{
    std::cout << "Assigning special mine arrays for type: " << type << " at (" << row << ", " << column << ")\n";
    if (type == 2) // Nuclear mines
    {
        for (int dr = -1; dr <= 1; ++dr)
        {
            for (int dc = -1; dc <= 1; ++dc)
            {
                int nr = row + dr;
                int nc = column + dc;
                if (nr >= 0 && nr < (int)b->rows && nc >= 0 && nc < (int)b->columns)
                {
                    if ((dr != 0 || dc != 0) && b->nuclear_array[nr][nc] != 2)
                    {
                        b->nuclear_array[nr][nc] = 1; // Mark surrounding cells as nuclear
                    }
                }
            }
        }
    }
    else if (type == 3) // Proximity mines
    {
        for (int dr = -1; dr <= 1; ++dr)
        {
            for (int dc = -1; dc <= 1; ++dc)
            {
                int nr = row + dr;
                int nc = column + dc;
                if (nr >= 0 && nr < (int)b->rows && nc >= 0 && nc < (int)b->columns)
                {
                    if ((dr != 0 || dc != 0) && b->proximity_array[nr][nc] != 2)
                    {
                        b->proximity_array[nr][nc] = 1; // Mark surrounding cells as nuclear
                    }
                }
            }
        }
    }
}

void board_draw(const struct Board *b)
{
    SDL_FRect dest_rect = {0, 0, b->piece_size, b->piece_size};
    for (unsigned row = 0; row < b->rows; row++)
    {
        dest_rect.y = (float)row * b->piece_size + b->rect.y;
        for (unsigned column = 0; column < b->columns; column++)
        {
            dest_rect.x = (float)column * b->piece_size + b->rect.x;
            unsigned index = b->front_array[row][column];
            SDL_RenderTexture(b->renderer, b->image, &b->src_rects[index], &dest_rect);
        }
    }
}
