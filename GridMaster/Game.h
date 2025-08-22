#pragma once
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <memory>
#include <iostream>
#include "Load_media.h"
#include "Border.h"
#include "Board.h"
#include "Clock.h"
#include "Mines.h"
#include "Face.h"
#include "Helper.h"
#include "Sound.h"

#define WINDOW_WIDTH 328
#define WINDOW_HEIGHT 414

#define PIECE_SIZE 16

#define BORDER_HEIGHT 55
#define BORDER_LEFT 4

#define DIGIT_BACK_WIDTH 41
#define DIGIT_BACK_HEIGHT 25
#define DIGIT_BACK_TOP 15
#define DIGIT_BACK_LEFT 15
#define DIGIT_BACK_RIGHT 3

#define DIGIT_WIDTH 13
#define DIGIT_HEIGHT 23

#define FACE_SIZE 26
#define FACE_TOP 15

class Game
{
  public:
    // Initialize all objects
    SDL_Window *window = nullptr;
    SDL_Window *windowHelper = nullptr;
    SDL_Renderer *renderer = nullptr;
    SDL_Renderer *rendererHelper = nullptr;
    struct Border *border = nullptr;
    struct Board *board = nullptr;
    struct Mines *mines = nullptr;
    struct Clock *clock = nullptr;
    struct Face *face = nullptr;
    struct Helper *helper = nullptr;
    struct Sound *sound = nullptr;
    SDL_Event event;

    // Game state variables
    bool isRunning = false;
    bool isPlaying = false;
    int gamePhase = 0; // How many times the player won essentially

    // Timing variables
    Uint64 lastFrameTime = 0;

    // Game configuration (Do not change Rows and Columns values, it is not implemented yet)
    unsigned int rows = 9;
    unsigned int columns = 9;
    unsigned int mineCount = 10;

    // Nuclear sensor
    Uint64 nuclearHoverStartTime = 0;
    Uint64 nuclearHoverDeathlyTime = 5000; // milliseconds
    bool isHoveringNuclear = false;
    int lastHoverNuclearRow = -1, lastHoverNuclearCol = -1;

    // WIndow shake variables
    bool isShaking = false;
    bool deathToogle = false;
    Uint64 shakeStartTime = 0;
    Uint64 shakeDuration = 500; // milliseconds
    int shakeMagnitude = 3;     // pixels
    int originalWindowX = 0, originalWindowY = 0;

    // Proximity sensor
    bool isHoveringProximity = false;
    int lastHoverProximityRow = -1, lastHoverProximityCol = -1;

    // Misc variables
    bool winToggle = false; // Toggle for win sound, so it doesn't play every frame

    bool Init(); // Initialize the game
    void Run();  // Main game loop

  private:
    void GameMouseDown(float x, float y, Uint8 button);
    bool GameMouseUp(float x, float y, Uint8 button);
    bool GameReset();    // Reset the board (It happens when mouse up in face)
    void WindowShake();  // Shake the window for radiation and losing
    void HandleEvents(); // Handles all the events / Inputs
    void UpdateState();  // Only updates FPS for now
    void Render();       // Render all the objects
    void Quit();
};
