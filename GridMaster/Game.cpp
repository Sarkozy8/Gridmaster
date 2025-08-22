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

    // Create the SDL window for Main Game
    window = SDL_CreateWindow("Grid Master", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_ALWAYS_ON_TOP);
    if (!window)
    {
        std::cerr << "SDL Window didnt create: " << SDL_GetError() << "\n";
        return false;
    }

    windowHelper = SDL_CreateWindow("Rules", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_ALWAYS_ON_TOP);
    if (!window)
    {
        std::cerr << "SDL Window for Helper didnt create: " << SDL_GetError() << "\n";
        return false;
    }

    // Set the position of the Helper window so they are not overlapping
    SDL_SetWindowPosition(windowHelper, 200, 200);

    // Create the SDL renderer for Main Game
    renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer)
    {
        std::cerr << "SDL Renderer didnt create: " << SDL_GetError() << "\n";
        return false;
    }

    rendererHelper = SDL_CreateRenderer(windowHelper, NULL);
    if (!rendererHelper)
    {
        std::cerr << "SDL Renderer for Helper didnt create: " << SDL_GetError() << "\n";
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
        if (!SDL_SetWindowIcon(windowHelper, iconSurface))
        {
            std::cerr << "Failed to set window icon for Helper: " << SDL_GetError() << "\n";
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

    // Initialize helper
    if (!helper_new(&helper, rendererHelper))
    {
        std::cerr << "Failed to create helper: " << SDL_GetError() << "\n";
        return false;
    }

    // Initialize sound
    if (!sound_new(&sound))
    {
        std::cerr << "Failed to create sound.\n";
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
    // Update mine count for different difficulty
    if (gamePhase > 1)
        mineCount = 15;
    if (gamePhase > 3)
        mineCount = 20;

    if (!board_reset(board, mineCount, true, gamePhase))
    {
        return false;
    }

    face_default(face);
    clock_reset(clock);
    mines_reset(mines, mineCount);
    helper_draw(helper, gamePhase); // Draw the helper with the initial game phase
    isPlaying = true;
    isHoveringNuclear = false;
    isHoveringProximity = false;
    deathToogle = false;

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
    std::cout << "\nnuclear_array:\n";
    for (unsigned row = 0; row < board->rows; ++row)
    {
        for (unsigned col = 0; col < board->columns; ++col)
        {
            std::cout << board->nuclear_array[row][col] << " ";
        }
        std::cout << "\n";
    }

    std::cout << "\nproximity_array:\n";
    for (unsigned row = 0; row < board->rows; ++row)
    {
        for (unsigned col = 0; col < board->columns; ++col)
        {
            std::cout << board->proximity_array[row][col] << " ";
        }
        std::cout << "\n";
    }

    // If pressed the up face with left mouse button, it resets the game
    if (button == SDL_BUTTON_LEFT)
    {

        if (face_mouse_click(face, x, y, false))
        {

            sound_play(sound, SOUND_CLICK);

            if (!GameReset())
            {
                // This tell on the event handler to change the isPLaying state to false
                return false;
            }
        }
    }

    if (isPlaying)
    {

        sound_play(sound, SOUND_CLICK);

        // If hit a mine, it changes the isPLaying state to false, also it changes the face state to lost,
        // and if the right mouse button is pressed, it marks the mine or unkown on top of it
        if (!board_mouse_up(board, x, y, button, gamePhase))
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

        if (gamePhase > 4 && winToggle == false)
        {
            sound_play(sound, SOUND_WIN_GRID_MASTER);
            sound_stop(sound, SOUND_RADIATION);
            winToggle = true;
            gamePhase++;
        }

        else if (winToggle == false)
        {
            sound_play(sound, SOUND_WIN);
            sound_stop(sound, SOUND_RADIATION);
            winToggle = true;
            gamePhase++;
        }
    }
    // Lost state
    else if (board_game_state(board) == GAME_LOST)
    {
        if (isHoveringNuclear)
            face_radiation(face);
        else
            face_lost(face);

        isPlaying = false;

        // Only shake the window if the game is lost once, not every click
        if (deathToogle == false)
        {
            isShaking = true;
            deathToogle = true;
            sound_play(sound, SOUND_EXPLOSION);
            sound_stop(sound, SOUND_RADIATION);
        }
    }
    // Reset Face
    else
    {
        face_default(face);
        winToggle = false;
    }

    // If not lost, we keep playing
    return true;
}

// Shake the window for radiation and losing
void Game::WindowShake()
{
    Uint64 now = SDL_GetTicks();
    if (now - shakeStartTime < shakeDuration)
    {
        int offsetX = (rand() % (2 * shakeMagnitude + 1)) - shakeMagnitude;
        int offsetY = (rand() % (2 * shakeMagnitude + 1)) - shakeMagnitude;
        SDL_SetWindowPosition(window, originalWindowX + offsetX, originalWindowY + offsetY);
    }
    else
    {
        SDL_SetWindowPosition(window, originalWindowX, originalWindowY);
        isShaking = false;
    }
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
        case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
            isRunning = false;
            break;
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            if (event.button.windowID == SDL_GetWindowID(window))
                GameMouseDown(event.button.x, event.button.y, event.button.button);
            break;
        case SDL_EVENT_MOUSE_BUTTON_UP:
            if (event.button.windowID == SDL_GetWindowID(window))
                if (!GameMouseUp(event.button.x, event.button.y, event.button.button))
                {
                    isPlaying = false;
                }
            break;
        case SDL_EVENT_MOUSE_MOTION:
            if (event.motion.windowID == SDL_GetWindowID(window))
            {
                if (board->game_state == GAME_PLAY)
                {
                    // Check if hovering over the nuclear mine
                    int hoverRow = -1, hoverCol = -1;
                    if (IsHoveringNuclear(board, event.motion.x, event.motion.y, hoverRow, hoverCol))
                    {
                        if (!isHoveringNuclear)
                        {
                            isHoveringNuclear = true;
                            isShaking = true;
                            lastHoverNuclearRow = hoverRow;
                            lastHoverNuclearCol = hoverCol;
                            nuclearHoverStartTime = SDL_GetTicks();
                        }
                    }
                    else
                    {
                        isHoveringNuclear = false;
                        lastHoverNuclearRow = -1;
                        lastHoverNuclearCol = -1;
                    }

                    // Check if hovering close to proximity mine
                    if (IsHoveringProximity(board, event.motion.x, event.motion.y, hoverRow, hoverCol))
                    {
                        if (!isHoveringProximity)
                        {
                            isHoveringProximity = true;
                            lastHoverProximityRow = hoverRow;
                            lastHoverProximityCol = hoverCol;
                        }
                    }
                    else
                    {
                        isHoveringProximity = false;
                        lastHoverProximityRow = -1;
                        lastHoverProximityCol = -1;
                    }
                    // Check if hover over proximity mine
                    if (HoverOverProximity(board, event.motion.x, event.motion.y, hoverRow, hoverCol))
                    {
                        std::cout << "You died because of Proximity Mine\n";
                        board->game_state = GAME_LOST;
                        isHoveringProximity = false;
                        board_reveal(board);
                        face_lost(face);
                        sound_play(sound, SOUND_EXPLOSION);
                        sound_stop(sound, SOUND_RADIATION);
                        isShaking = true;
                        isPlaying = false;
                        deathToogle = true;
                    }
                }
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

// Logic for the game states
void Game::UpdateState()
{
    // If hovering over a nuclear mine for more than 10 seconds, end the game
    if (isHoveringNuclear && (SDL_GetTicks() - nuclearHoverStartTime) >= nuclearHoverDeathlyTime &&
        board->game_state == GAME_PLAY)
    {
        std::cout << "You died\n";
        board->game_state = GAME_LOST;
        isHoveringNuclear = true;
        board_reveal(board);
        face_radiation(face);
        sound_play(sound, SOUND_EXPLOSION);
        sound_stop(sound, SOUND_RADIATION);
        isPlaying = false;
        deathToogle = true;
    }

    // If hovering near a proximity mine, play radiation sound
    if (isHoveringNuclear && board->game_state == GAME_PLAY)
    {
        sound_play(sound, SOUND_RADIATION);
    }
    else if (!isHoveringNuclear && board->game_state == GAME_PLAY)
    {
        sound_stop(sound, SOUND_RADIATION);
    }

    // If hovering near a proximity mine, play beep sound
    if (isHoveringProximity && board->game_state == GAME_PLAY)
    {
        sound_play(sound, SOUND_BEEP);
    }

    // Update window position values before shaking
    if (!isShaking)
    {
        shakeStartTime = SDL_GetTicks();
        SDL_GetWindowPosition(window, &originalWindowX, &originalWindowY);
    }

    // Keeps shaking the window if hovering over a nuclear mine but stops after death
    if (isHoveringNuclear && board->game_state == GAME_PLAY)
        isShaking = true;

    // Check windows if it should shake
    if (isShaking)
        WindowShake();

    // Calculate delta time
    Uint64 deltaTime = SDL_GetTicks() - lastFrameTime;

    // Limit the frame rate to 60 FPS
    if (deltaTime < 16) // Reason for 16: 1000ms / 60fps = ~16ms per frame
    {
        SDL_Delay(static_cast<Uint32>(16 - deltaTime));
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
    SDL_DestroyWindow(windowHelper);
    windowHelper = nullptr;
    SDL_DestroyRenderer(rendererHelper);
    rendererHelper = nullptr;
    border_free(&border);
    board_free(&board);
    mines_free(&mines);
    clock_free(&clock);
    face_free(&face);
    helper_free(&helper);
    sound_free(&sound);
    SDL_Quit();
}
