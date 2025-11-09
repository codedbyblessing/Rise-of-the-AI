#include "LevelC.h"
#include "raylib.h"

const float LevelC::C_TILE_UNIT  = 80.0f;
const float LevelC::C_FALL_FORCE = 1020.0f;
const float LevelC::C_EXIT_LIMIT = 1000.0f;

LevelC::LevelC()                                      : Scene({0.0f,0.0f}, nullptr) {}
LevelC::LevelC(Vector2 origin, const char* bgHexCode) : Scene(origin, bgHexCode) {}
LevelC::~LevelC() { shutdown(); }

void LevelC::initialise()
{
    mGameState.nextSceneID = -1;
    // ---------- MAP ----------
    mGameState.map = new Map(
        C_WIDTH, C_HEIGHT,
        (unsigned int*)mTileMatrixC,
        "assets/World_tileset.png",
        C_TILE_UNIT,
        17, 12,
        mOrigin
    );
    // ---------- dude ----------
    std::map<Direction, std::vector<int>> dudeAtlas = {
        {LEFT,  {0,1,2,3,4,5,6,7,8,9}},
        {RIGHT, {0,1,2,3,4,5,6,7,8,9}},
        {UP,    {0}},
        {DOWN,  {0}}
    };

    float sizeRatio = 1.0f;

    mGameState.character = new Entity(
        {mOrigin.x - 250.0f, mOrigin.y - 250.0f},
        {250.0f * sizeRatio, 250.0f},
        "assets/Dude/WalkDude.png",
        ATLAS,
        {16, 1},
        dudeAtlas,
        PLAYER
    );

    mGameState.character->setJumpingPower(550.0f);
    mGameState.character->setColliderDimensions({
        mGameState.character->getScale().x / 3.5f,
        mGameState.character->getScale().y / 3.0f
    });
    mGameState.character->setAcceleration({0.0f, C_FALL_FORCE});

    // ---------- CAMERA ----------
    mGameState.camera = {0};
    mGameState.camera.target = mGameState.character->getPosition();
    mGameState.camera.offset = mOrigin;
    mGameState.camera.rotation = 0.0f;
    mGameState.camera.zoom = 1.0f;
}

void LevelC::update(float deltaTime)
{
    if (IsKeyDown(KEY_W)) mGameState.character->setTexture("assets/Dude/JumpDude.png");
    else                  mGameState.character->setTexture("assets/Dude/WalkDude.png");

    mGameState.character->update(deltaTime, nullptr, mGameState.map, nullptr, 0);

    Vector2 camTarget = { mGameState.character->getPosition().x, mOrigin.y };
    if (mGameState.character->getPosition().y > C_EXIT_LIMIT)
        mGameState.nextSceneID = 0; //scene 0 is the menu

    panCamera(&mGameState.camera, &camTarget);
}

void LevelC::render()
{
    ClearBackground(ColorFromHex(mBGColourHexCode));
    if (mGameState.map)       mGameState.map->render();
    if (mGameState.character) mGameState.character->render();
}

void LevelC::shutdown()
{
    delete mGameState.character; mGameState.character = nullptr;
    delete mGameState.map;       mGameState.map       = nullptr;
}
