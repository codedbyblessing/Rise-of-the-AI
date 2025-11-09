#ifndef SCENE_H
#define SCENE_H
#include "Entity.h"
#include "Map.h"
#include "raylib.h"

struct GameState
{
    Entity* character = nullptr;
    Entity* zombie = nullptr;
    Map* map = nullptr;

    Music bgm;
    Music gameMusic;

    Sound walkingSfx;

    Camera2D camera;

    int nextSceneID = -1;
    int playerLives = 3;

    bool gameWon = false;
    bool gameLost = false;

    bool walkingLooping = false;
};

class Scene
{
protected:
    GameState mGameState;
    Vector2 mOrigin;
    const char* mBGColourHexCode;

public:
    Scene();
    Scene(Vector2 origin, const char* bgHexCode);
    virtual ~Scene() = default;

    virtual void initialise() = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render() = 0;
    virtual void shutdown() {}

    GameState& getState() { return mGameState; }
    Vector2 getOrigin() const { return mOrigin; }
    const char* getBGColourHexCode() const { return mBGColourHexCode; }
};

#endif
