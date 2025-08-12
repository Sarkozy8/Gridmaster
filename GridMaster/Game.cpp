#include "Game.h"

bool Game::Init()
{
    // Seed the random number generator
    srand((unsigned)time(NULL));

    // Initialize SDL
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
    {
        std::cerr << "SDL didnt initialize: " << SDL_GetError() << "\n";
        return false;
    }

    // Create the SDL window
    window = SDL_CreateWindow("Grid Master", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_ALWAYS_ON_TOP);
    if (!window)
    {
        std::cerr << "SDL Window didnt create: " << SDL_GetError() << "\n";
        return false;
    }

    // Create the SDL renderer
    renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer)
    {
        std::cerr << "SDL Renderer didnt create: " << SDL_GetError() << "\n";
        return false;
    }

    // Loads the icon for the window
    SDL_Surface *iconSurface = IMG_Load("assets/bombIcon.png");
    if (iconSurface)
    {
        if (!SDL_SetWindowIcon(window, iconSurface))
        {
            std::cerr << "Failed to set window icon: " << SDL_GetError() << "\n";
            SDL_DestroySurface(iconSurface);
            return false;
        }
    }
    else
    {
        std::cerr << "Failed to load icon surface: " << SDL_GetError() << "\n";
        SDL_DestroySurface(iconSurface);
        return false;
    }

    SDL_DestroySurface(iconSurface); // Clean up the icon surface after setting the icon

    // Initialize border
    if (!border_new(&border, renderer, rows, columns))
    {
        std::cerr << "Failed to create border: " << SDL_GetError() << "\n";
        return false;
    }

    // Initialize board
    if (!board_new(&board, renderer, rows, columns, mineCount))
    {
        std::cerr << "Failed to create border: " << SDL_GetError() << "\n";
        return false;
    }

    // Initialize mines
    if (!mines_new(&mines, renderer, mineCount))
    {
        std::cerr << "Failed to create mines: " << SDL_GetError() << "\n";
        return false;
    }

    // Initialize clock
    if (!clock_new(&clock, renderer, columns))
    {
        std::cerr << "Failed to create clock: " << SDL_GetError() << "\n";
        return false;
    }
    // Initialize face
    if (!face_new(&face, renderer, columns))
    {
        std::cerr << "Failed to create face: " << SDL_GetError() << "\n";
        return false;
    }

    // Set the initial state
    isRunning = true;

    // Reset the game state to get a fresh start
    GameReset();

    return true; // Return true if everything initialized successfully
}

// Reset everything
bool Game::GameReset()
{
    if (!board_reset(board, mineCount, true))
    {
        return false;
    }

    face_default(face);
    clock_reset(clock);
    mines_reset(mines, mineCount);
    isPlaying = true;

    return true;
}

// Mouse Down event handler
void Game::GameMouseDown(float x, float y, Uint8 button)
{
    // This only changes the face state when the left mouse button is pressed
    if (button == SDL_BUTTON_LEFT)
    {
        face_mouse_click(face, x, y, true);
    }

    // If you are playing, it only changes the face state in the case of left mouse button
    if (isPlaying)
    {
        board_mouse_down(board, x, y, button);
        if (board_is_pressed(board))
        {
            face_question(face);
        }
    }
}

// Mouse up event handler
bool Game::GameMouseUp(float x, float y, Uint8 button)
{
    // If pressed the up face with left mouse button, it resets the game
    if (button == SDL_BUTTON_LEFT)
    {
        if (face_mouse_click(face, x, y, false))
        {
            if (!GameReset())
            {
                // This tell on the event handler to change the isPLaying state to false
                return false;
            }
        }
    }

    if (isPlaying)
    {
        // If hit a mine, it changes the isPLaying state to false, also it changes the face state to lost,
        // and if the right mouse button is pressed, it marks the mine or unkown on top of it
        if (!board_mouse_up(board, x, y, button))
        {
            return false;
        }

        // Changes mine count everytime is marked
        if (board_mine_marked(board) == 1)
        {
            mines_increment(mines);
        }
        else if (board_mine_marked(board) == -1)
        {
            mines_decrement(mines);
        }
    }

    // Won state
    if (board_game_state(board) == GAME_WON)
    {
        face_won(face);
        mines_reset(mines, 0);
        isPlaying = false;
    }
    // Lost state
    else if (board_game_state(board) == GAME_LOST)
    {
        face_lost(face);
        isPlaying = false;
    }
    // Reset Face
    else
    {
        face_default(face);
    }

    // If not lost, we keep playing
    return true;
}

// Main game loop
void Game::Run()
{
    while (isRunning)
    {
        HandleEvents();
        UpdateState();
        Render();
    }
    Quit();
}

// Handle all the events
void Game::HandleEvents()
{
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_EVENT_QUIT:
            isRunning = false;
            break;
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            GameMouseDown(event.button.x, event.button.y, event.button.button);
            break;
        case SDL_EVENT_MOUSE_BUTTON_UP:
            if (!GameMouseUp(event.button.x, event.button.y, event.button.button))
            {
                isPlaying = false;
            }
            break;
        case SDL_EVENT_KEY_DOWN:
            switch (event.key.scancode)
            {
            case SDL_SCANCODE_ESCAPE:
                isRunning = false;
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
    }
}

// FPS for now amd start the clock
void Game::UpdateState()
{
    // Calculate delta time
    Uint64 deltaTime = SDL_GetTicks() - lastFrameTime;

    // Limit the frame rate to 60 FPS
    if (deltaTime < 16) // Reason for 16: 1000ms / 60fps = ~16ms per frame
    {
        SDL_Delay(16 - deltaTime);
    }

    lastFrameTime = SDL_GetTicks(); // Update last frame time

    if (isPlaying)
    {
        clock_update(clock);
    }
}

// Render all the objects
void Game::Render()
{
    SDL_RenderClear(renderer);

    border_draw(border);
    board_draw(board);
    mines_draw(mines);
    clock_draw(clock);
    face_draw(face);

    SDL_RenderPresent(renderer); // Swap buffers
}

// Clean up and quit the game
void Game::Quit()
{
    SDL_DestroyWindow(window);
    window = nullptr;
    SDL_DestroyRenderer(renderer);
    renderer = nullptr;
    border_free(&border);
    board_free(&board);
    mines_free(&mines);
    clock_free(&clock);
    face_free(&face);
    SDL_Quit();
}
