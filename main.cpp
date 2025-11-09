/**
* Author: Blessing Yeboah
* Assignment: Rise of the AI
* Date due: 2025-11-08, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "CS3113/LevelA.h"
#include "CS3113/LevelB.h"
#include "CS3113/LevelC.h"
#include "CS3113/Menu.h"
#include "CS3113/Scene.h"
#include "CS3113/cs3113.h"
#include <vector>

const int SCREEN_WIDTH     = 1000;
const int SCREEN_HEIGHT    = 600;
const int FPS              = 120;
const int NUMBER_OF_LEVELS = 4;
const Vector2 ORIGIN       = { SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };
const float FIXED_TIMESTEP = 1.0f / 60.0f;

float gPreviousTicks   = 0.0f;
float gTimeAccumulator = 0.0f;

Scene *gCurrentScene = nullptr;
std::vector<Scene*> gLevels;

MenuScene *gMenu  = nullptr;
LevelA *gLevelA   = nullptr;
LevelB *gLevelB   = nullptr;
LevelC *gLevelC   = nullptr;

void switchToScene(Scene *scene)
{
    gCurrentScene = scene;
    gCurrentScene->initialise();
}

void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Rise of the AI");
    InitAudioDevice();
    gMenu  = new MenuScene(ORIGIN, "#000000");
    gLevelA = new LevelA(ORIGIN, "#202430");
    gLevelB = new LevelB(ORIGIN, "#112233");
    gLevelC = new LevelC(ORIGIN, "#001F3F");
    gLevels.push_back(gMenu);
    gLevels.push_back(gLevelA);
    gLevels.push_back(gLevelB);
    gLevels.push_back(gLevelC);
    switchToScene(gLevels[0]);
    SetTargetFPS(FPS);
}

void processInput()
{
}

void update()
{
    float ticks = (float) GetTime();
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks  = ticks;
    deltaTime += gTimeAccumulator;
    if (deltaTime < FIXED_TIMESTEP) { gTimeAccumulator = deltaTime; return; }
    while (deltaTime >= FIXED_TIMESTEP)
    {
        gCurrentScene->update(FIXED_TIMESTEP);
        deltaTime -= FIXED_TIMESTEP;
    }
}

void render()
{
    BeginDrawing();
    BeginMode2D(gCurrentScene->getState().camera);
    gCurrentScene->render();
    EndMode2D();
    EndDrawing();
}

void shutdown()
{
    delete gMenu;  gMenu = nullptr;
    delete gLevelA; gLevelA = nullptr;
    delete gLevelB; gLevelB = nullptr;
    delete gLevelC; gLevelC = nullptr;
    for (int i = 0; i < NUMBER_OF_LEVELS; i++) gLevels[i] = nullptr;
    CloseAudioDevice();
    CloseWindow();
}

int main(void)
{
    initialise();
    while (true)
    {
        if (WindowShouldClose()) break;
        processInput();
        update();
        if (gCurrentScene->getState().nextSceneID >= 0)
        {
            int id = gCurrentScene->getState().nextSceneID;
            if (id >= 0 && id < (int)gLevels.size()) switchToScene(gLevels[id]);
        }
        render();
    }
    shutdown();
    return 0;
}
