#include "CS3113/Entity.h"

enum GameMode {
    MENU,
    PLAYING,
    WIN
};

GameMode gMode = MENU;

struct GameState
{
    Entity *xochitl;
    Entity *button;    // fruit 1
    Entity *button2;   // fruit 2
    Entity *coconut;   // fruit 3

    Map *map;

    Music bgm;
    Sound jumpSound;
    Sound fruitSound;
    Sound walking;
    Sound menuScreen;


    Camera2D camera;
};

constexpr int SCREEN_WIDTH  = 1000;
constexpr int SCREEN_HEIGHT = 600;
constexpr int FPS = 120;

constexpr char BG_COLOUR[] = "#011627";

constexpr Vector2 ORIGIN = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
constexpr Vector2 ATLAS_DIMENSIONS = { 6, 8 };

constexpr float TILE_DIMENSION = 75.f;
constexpr float GRAVITY = 981.f;
constexpr float FIXED_TIMESTEP = 1.f / 60.f;

constexpr int LEVEL_WIDTH  = 16;
constexpr int LEVEL_HEIGHT = 39;

AppStatus gAppStatus = RUNNING;
float gPreviousTicks = 0.f;
float gTimeAccumulator = 0.f;

Color gBackgroundColor = ColorFromHex(BG_COLOUR);

unsigned int gLevelData[] = {


    1, 0, 1, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
    1, 0, 1, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 5, 0, 5,
    1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 5,
    1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5,
    1, 0, 1, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 5, 0, 5,
    1, 0, 1, 0, 0, 0, 5, 0, 0, 0, 0, 0, 5, 0, 0, 5,
    1, 0, 1, 5, 0, 0, 0, 0, 0, 0, 5, 5, 0, 0, 0, 5,
    1, 0, 1, 0, 0, 0, 5, 5, 0, 0, 0, 0, 0, 0, 0, 5,
    1, 0, 1, 0, 0, 0, 0, 0, 0, 5, 0, 5, 5, 2, 2, 5,
    1, 0, 1, 0, 0, 0, 5, 0, 0, 0, 0, 0, 5, 0, 0, 5,
    1, 0, 1, 5, 0, 5, 0, 0, 0, 5, 5, 5, 0, 0, 0, 5,
    1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 1, 0, 0, 5, 0, 0, 5, 0, 0, 5, 0, 5, 2, 5,
    1, 0, 1, 0, 0, 0, 5, 0, 2, 0, 0, 0, 0, 5, 2, 5,
    1, 0, 1, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 5,
    1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 5,
    1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5,
    1, 0, 1, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 5, 0, 5,
    1, 0, 1, 0, 0, 0, 5, 0, 0, 0, 0, 0, 5, 0, 0, 5,
    1, 0, 1, 5, 0, 0, 0, 0, 0, 0, 5, 5, 0, 0, 0, 5,
    1, 0, 1, 0, 0, 0, 5, 5, 0, 0, 0, 0, 0, 0, 0, 5,
    1, 0, 1, 0, 0, 0, 0, 0, 0, 5, 0, 5, 5, 2, 2, 5,
    1, 0, 1, 0, 0, 0, 5, 0, 0, 0, 0, 0, 5, 0, 0, 5,
    1, 0, 1, 5, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 5,
    1, 0, 1, 0, 0, 0, 5, 5, 0, 0, 0, 0, 5, 0, 0, 5,
    1, 0, 1, 0, 0, 0, 0, 0, 5, 0, 0, 0, 5, 2, 2, 5,
    1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 1, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 5,
    1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 5,
    1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5,
    1, 0, 1, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 5, 0, 5,
    1, 0, 1, 0, 0, 0, 5, 0, 0, 0, 0, 0, 5, 0, 0, 5,
    1, 0, 1, 5, 0, 0, 0, 0, 0, 0, 5, 5, 0, 0, 0, 5,
    1, 0, 1, 0, 0, 0, 5, 5, 0, 0, 0, 0, 0, 0, 0, 5,
    1, 0, 1, 0, 0, 0, 0, 0, 0, 5, 0, 5, 5, 2, 2, 5,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5,
    1, 2, 0, 2, 2, 2, 2, 2, 2, 2, 5, 5, 5, 5, 5, 5,
    1, 5, 5, 3, 3, 3, 3, 3, 3, 3, 5, 5, 5, 5, 5, 5,
    1, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5
};


Rectangle getCollision(Entity* e)
{
    Vector2 pos  = e->getPosition();
    Vector2 size = e->getColliderDimensions();
    return Rectangle{
        pos.x - size.x * 0.5f,
        pos.y - size.y * 0.5f,
        size.x,
        size.y
    };
}

void panCamera(Camera2D *camera, const Vector2 *target)
{
    const float lerp = 0.1f;
    camera->target.x += (target->x - camera->target.x) * lerp;
    camera->target.y += (target->y - camera->target.y) * lerp;
}

GameState gState;

void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Maps");
    InitAudioDevice();

    gState.bgm = LoadMusicStream("assets/game/LoadingMusic.mp3");
    SetMusicVolume(gState.bgm, 0.33f);

    gState.jumpSound = LoadSound("assets/game/jump.mp3");
    gState.fruitSound = LoadSound("assets/game/eat.mp3");
    gState.walking = LoadSound("assets/game/Walking.mp3");
    gState.menuScreen = LoadSound("assets/game/LoadingMusic.mp3");


    gState.map = new Map(
        LEVEL_WIDTH, LEVEL_HEIGHT,
        (unsigned int*)gLevelData,
        "assets/game/tileset.png",
        TILE_DIMENSION, 4, 1, ORIGIN
    );

    std::map<Direction, std::vector<int>> atlas =
    {
        {DOWN,  {0,1,2,3,4,5,6,7}},
        {LEFT,  {8,9,10,11,12,13,14,15}},
        {UP,    {24,25,26,27,28,29,30,31}},
        {RIGHT, {40,41,42,43,44,45,46,47}}
    };

    float sizeRatio = 48.f / 64.f;

    float spawnX = gState.map->getLeftBoundary() + TILE_DIMENSION * 1.5f;
    float spawnY = gState.map->getTopBoundary()  + TILE_DIMENSION * 0.5f;

    gState.xochitl = new Entity(
        {spawnX, spawnY},
        {200.f * sizeRatio, 200.f},
        "assets/game/walk.png",
        ATLAS, ATLAS_DIMENSIONS,
        atlas, PLAYER
    );

    gState.xochitl->setColliderDimensions({
        gState.xochitl->getScale().x / 3.5f,
        gState.xochitl->getScale().y / 3.0f
    });
    gState.xochitl->setAcceleration({0, GRAVITY});
    gState.xochitl->setJumpingPower(550.f);

    int c1row = 8,  c1col = 3;
    int c2row = 16, c2col = 3;
    int c3row = 30, c3col = 5;

    float c1x = gState.map->getLeftBoundary() + (c1col + .5f)*TILE_DIMENSION;
    float c1y = gState.map->getTopBoundary()  + (c1row + .5f)*TILE_DIMENSION;
    gState.button = new Entity({c1x,c1y},{TILE_DIMENSION,TILE_DIMENSION},
                               "assets/game/fruit.png",BLOCK);

    float c2x = gState.map->getLeftBoundary() + (c2col + .5f)*TILE_DIMENSION;
    float c2y = gState.map->getTopBoundary()  + (c2row + .5f)*TILE_DIMENSION;
    gState.button2 = new Entity({c2x,c2y},{TILE_DIMENSION,TILE_DIMENSION},
                                "assets/game/pineapple.png",BLOCK);

    float c3x = gState.map->getLeftBoundary() + (c3col + .5f)*TILE_DIMENSION;
    float c3y = gState.map->getTopBoundary()  + (c3row + .5f)*TILE_DIMENSION;
    gState.coconut = new Entity({c3x,c3y},{TILE_DIMENSION,TILE_DIMENSION},
                                "assets/game/coconut.png",BLOCK);

    gState.button->setColliderDimensions({50,50});
    gState.button2->setColliderDimensions({60,60});
    gState.coconut->setColliderDimensions({40,40});

    gState.camera = {0};
    gState.camera.target = gState.xochitl->getPosition();
    gState.camera.offset = ORIGIN;
    gState.camera.zoom = 1;

    SetTargetFPS(FPS);
}

void processInput()
{
    if (gMode == MENU)
    {
        if (IsKeyPressed(KEY_ENTER))
            gMode = PLAYING;

        return;
    }
    if (gMode == WIN)
    {
        if (IsKeyPressed(KEY_Q))
            gAppStatus = TERMINATED;
        return;
    }

    gState.xochitl->resetMovement();

    if      (IsKeyDown(KEY_A)) 
    {
        gState.xochitl->moveLeft();
        PlaySound(gState.walking);
    }
    else if (IsKeyDown(KEY_D))
    {
        gState.xochitl->moveRight();
        PlaySound(gState.walking);
    }

    if (IsKeyPressed(KEY_W) && gState.xochitl->isCollidingBottom())
    {
        gState.xochitl->jump();
        PlaySound(gState.jumpSound);
    }

    if (IsKeyPressed(KEY_Q) || WindowShouldClose())
        gAppStatus = TERMINATED;
}

void update()
{
    if (gMode != PLAYING) return;

    float ticks = GetTime();
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks = ticks;

    deltaTime += gTimeAccumulator;

    if (deltaTime < FIXED_TIMESTEP)
    {
        gTimeAccumulator = deltaTime;
        return;
    }

    while (deltaTime >= FIXED_TIMESTEP)
    {
        gState.xochitl->update(FIXED_TIMESTEP, nullptr, gState.map, nullptr, 0);

        if (gState.button2->isActive() &&
            CheckCollisionRecs(getCollision(gState.xochitl), getCollision(gState.button2)))
        {
            gState.button2->deactivate();
            PlaySound(gState.fruitSound);


            int row10 = 11;
            int start1 = row10 * LEVEL_WIDTH;
            for (int i = 0; i < LEVEL_WIDTH; i++)
                gState.map->setTileType(start1 + i, 0);
        }

        if (gState.coconut->isActive() &&
            CheckCollisionRecs(getCollision(gState.xochitl), getCollision(gState.coconut)))
        {
            gState.coconut->deactivate();
            PlaySound(gState.fruitSound);


            int row25 = 26;
            int start2 = row25 * LEVEL_WIDTH;
            for (int i = 0; i < LEVEL_WIDTH; i++)
                gState.map->setTileType(start2 + i, 0);
        }

        if (gState.button->isActive() &&
            CheckCollisionRecs(getCollision(gState.xochitl), getCollision(gState.button)))
        {
            gState.button->deactivate();
            PlaySound(gState.fruitSound);
        


            int row32 = 33;
            int start3 = row32 * LEVEL_WIDTH;
            for (int i = 0; i < LEVEL_WIDTH; i++)
                gState.map->setTileType(start3 + i, 0);
        }

        deltaTime -= FIXED_TIMESTEP;
        Vector2 pos = gState.xochitl->getPosition();
        panCamera(&gState.camera, &pos);

        float row25y = gState.map->getTopBoundary() + TILE_DIMENSION * 25;

        if (gState.camera.target.y > row25y)
            gBackgroundColor = ColorFromHex("#003844");
        else
            gBackgroundColor = ColorFromHex(BG_COLOUR);
    }

    float POSy  = gState.xochitl->getPosition().y;
    float top = gState.map->getTopBoundary();

    if (POSy < top && !gState.button->isActive()  && !gState.button2->isActive() &&
    !gState.coconut->isActive())
    {
        gMode = WIN;
        return;
    }

    float px = gState.xochitl->getPosition().x;

    float leftBound  = gState.map->getLeftBoundary();
    float rightBound = gState.map->getRightBoundary();

    if ((px < leftBound || px > rightBound) &&  (!gState.button->isActive() && !gState.button2->isActive() &&
    !gState.coconut->isActive()))
    {
        gMode = WIN;
        return;
    }
}

void render()
{
    if (gMode == MENU)
    {
        BeginDrawing();
        ClearBackground(BLACK);

        DrawText("RISE OF THE AI", 200, 200, 50, WHITE);
        DrawText("Press ENTER to Begin", 260, 330, 30, WHITE);
        PlaySound(gState.menuScreen);


        EndDrawing();
        return;
    }
    if (gMode == WIN)
    {
        BeginDrawing();
        ClearBackground(ColorFromHex("#b977c7ff"));

        DrawText("CONGRATULATIONS!", 220, 200, 50, WHITE);
        DrawText("You Escaped!", 340, 300, 40, WHITE);
        DrawText("Press Q to Quit", 350, 400, 30, WHITE);

        EndDrawing();
        return;
    }

    BeginDrawing();
    ClearBackground(gBackgroundColor);

    BeginMode2D(gState.camera);

    gState.map->render();
    gState.xochitl->render();

    if (gState.button->isActive())  gState.button->render();
    if (gState.button2->isActive()) gState.button2->render();
    if (gState.coconut->isActive()) gState.coconut->render();

    EndMode2D();
    EndDrawing();
}

void shutdown()
{
    delete gState.xochitl;
    delete gState.button;
    delete gState.button2;
    delete gState.coconut;
    delete gState.map;

    UnloadMusicStream(gState.bgm);
    UnloadSound(gState.jumpSound);

    CloseAudioDevice();
    CloseWindow();
}

int main()
{
    initialise();

    while (gAppStatus == RUNNING)
    {
        processInput();
        update();
        render();
    }

    shutdown();
    return 0;
}
