#ifndef SCENE_H
#define SCENE_H

#include "Entity.h"
#include "raylib.h"
#include <map>
#include <vector>

// ------------ GAME MODE ------------
enum GameMode {
    MENU,
    PLAYING,
    WIN
};

// ------------ GLOBAL MODE ------------
inline GameMode gMode = MENU;

// ------------ GLOBAL STATE STRUCT ------------
struct GameState
{
    Entity *xochitl;
    Entity *button;
    Entity *button2;
    Entity *coconut;

    Map *map;

    Music bgm;
    Sound jumpSound;
    Sound fruitSound;
    Sound walking;
    Sound menuScreen;

    Camera2D camera;
};

inline GameState gState;

// ------------ CONSTANTS ------------
inline constexpr int SCREEN_WIDTH  = 1000;
inline constexpr int SCREEN_HEIGHT = 600;
inline constexpr int FPS = 120;

inline constexpr char BG_COLOUR[] = "#011627";

inline constexpr float TILE_DIMENSION = 75.f;
inline constexpr float GRAVITY = 981.f;
inline constexpr float FIXED_TIMESTEP = 1.f / 60.f;

inline constexpr int LEVEL_WIDTH  = 16;
inline constexpr int LEVEL_HEIGHT = 39;

inline constexpr Vector2 ORIGIN = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
inline constexpr Vector2 ATLAS_DIMENSIONS = { 6, 8 };

// ------------ GLOBALS ------------
inline AppStatus gAppStatus = RUNNING;
inline float gPreviousTicks = 0.f;
inline float gTimeAccumulator = 0.f;
inline Color gBackgroundColor = ColorFromHex(BG_COLOUR);


inline unsigned int gLevelData[] = {
    1,0,1,5,5,5,5,5,5,5,5,5,5,5,5,5,
    1,0,1,0,0,0,0,0,0,5,0,0,0,5,0,5,
    1,0,1,0,0,0,0,0,0,0,0,5,0,0,0,5,
    1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,5,
    1,0,1,0,0,0,0,0,5,0,0,0,0,5,0,5,
    1,0,1,0,0,0,5,0,0,0,0,0,5,0,0,5,
    1,0,1,5,0,0,0,0,0,0,5,5,0,0,0,5,
    1,0,1,0,0,0,5,5,0,0,0,0,0,0,0,5,
    1,0,1,0,0,0,0,0,0,5,0,5,5,2,2,5,
    1,0,1,0,0,0,5,0,0,0,0,0,5,0,0,5,
    1,0,1,5,0,5,0,0,0,5,5,5,0,0,0,5,
    1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,0,1,0,0,5,0,0,5,0,0,5,0,5,2,5,
    1,0,1,0,0,0,5,0,2,0,0,0,0,5,2,5,
    1,0,1,0,0,0,0,0,0,2,0,0,0,0,0,5,
    1,0,1,0,0,0,0,0,0,0,0,5,0,0,0,5,
    1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,5,
    1,0,1,0,0,0,0,0,5,0,0,0,0,5,0,5,
    1,0,1,0,0,0,5,0,0,0,0,0,5,0,0,5,
    1,0,1,5,0,0,0,0,0,0,5,5,0,0,0,5,
    1,0,1,0,0,0,5,5,0,0,0,0,0,0,0,5,
    1,0,1,0,0,0,0,0,0,5,0,5,5,2,2,5,
    1,0,1,0,0,0,5,0,0,0,0,0,5,0,0,5,
    1,0,1,5,0,0,0,0,0,0,5,0,0,0,0,5,
    1,0,1,0,0,0,5,5,0,0,0,0,5,0,0,5,
    1,0,1,0,0,0,0,0,5,0,0,0,5,2,2,5,
    1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,0,1,0,0,0,0,0,0,5,0,0,0,0,0,5,
    1,0,1,0,0,0,0,0,0,0,0,5,0,0,0,5,
    1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,5,
    1,0,1,0,0,0,0,0,5,0,0,0,0,5,0,5,
    1,0,1,0,0,0,5,0,0,0,0,0,5,0,0,5,
    1,0,1,5,0,0,0,0,0,0,5,5,0,0,0,5,
    1,0,1,0,0,0,5,5,0,0,0,0,0,0,0,5,
    1,0,1,0,0,0,0,0,0,5,0,5,5,2,2,5,
    1,0,0,0,0,0,0,0,0,0,0,5,5,5,5,5,
    1,2,0,2,2,2,2,2,2,2,5,5,5,5,5,5,
    1,5,5,3,3,3,3,3,3,3,5,5,5,5,5,5,
    1,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5
};

Rectangle getCollision(Entity* e);
void panCamera(Camera2D *camera, const Vector2 *target);

void initialise();
void processInput();
void update();
void render();
void shutdown();

#endif
