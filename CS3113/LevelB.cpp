#include "LevelB.h"

const float LevelB::B_TILE_SIZE        = 70.0f;
const float LevelB::B_GRAVITY_PULL     = 915.0f;
const float LevelB::B_FINISH_TRIGGER   = 900.0f;

LevelB::LevelB() : Scene({0,0}, "#000000"), mZombie(nullptr)
{
    unsigned int temp[B_COLUMNS * B_ROWS] = {
        4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
        4,14,14,14,14,14,14,14,14,14,14,14,14,14,14,4,
        4,14,2,2,2,14,14,14,3,3,14,14,2,2,14,4,
        4,14,14,14,2,14,14,14,3,3,14,14,14,2,14,4,
        4,14,14,14,2,14,14,14,14,14,14,14,14,2,14,4,
        4,14,14,14,14,14,3,3,3,3,3,14,14,14,14,4,
        4,14,3,3,3,14,14,14,14,14,14,14,3,3,14,4,
        4,14,3,3,3,14,14,14,14,14,14,14,3,3,14,4,
        4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4
    };

}

LevelB::LevelB(Vector2 origin, const char* bgHex)
: Scene(origin, bgHex), mZombie(nullptr)
{
    unsigned int temp[B_COLUMNS * B_ROWS] = {
        4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
        4,14,14,14,14,14,14,14,14,14,14,14,14,14,14,4,
        4,14,2,2,2,14,14,14,3,3,14,14,2,2,14,4,
        4,14,14,14,2,14,14,14,3,3,14,14,14,2,14,4,
        4,14,14,14,2,14,14,14,14,14,14,14,14,2,14,4,
        4,14,14,14,14,14,3,3,3,3,3,14,14,14,14,4,
        4,14,3,3,3,14,14,14,14,14,14,14,3,3,14,4,
        4,14,3,3,3,14,14,14,14,14,14,14,3,3,14,4,
        4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4
    };

    memcpy(mGridB, temp, sizeof(temp));
}

LevelB::~LevelB() {
    shutdown();
}

void LevelB::initialise()
{
    mGameState.nextSceneID = -1;

    mGameState.bgm = LoadMusicStream("assets/GameMusic.mp3");
    PlayMusicStream(mGameState.bgm);

    mGameState.map = new Map(
        B_COLUMNS, B_ROWS,
        mGridB,
        "assets/World_tileset.png",
        B_TILE_SIZE,
        17, 12,
        mOrigin
    );

    // Player atlas
    std::map<Direction, std::vector<int>> dudeAtlas = {
        {DOWN,  {0,1,2,3,4,5}},
        {LEFT,  {0,1,2,3,4,5}},
        {UP,    {0,1,2,3,4,5}},
        {RIGHT, {0,1,2,3,4,5}}
    };

    Vector2 dudeSize = { 250.0f * (48.0f/64.0f), 250.0f };
    Vector2 dudeStart = { mOrigin.x - 280.0f, mOrigin.y - 190.0f };

    mGameState.character = new Entity(
        dudeStart,
        dudeSize,
        "assets/Dude/IdleDude.png",
        ATLAS,
        {1,6},
        dudeAtlas,
        PLAYER
    );
    mGameState.character->setAcceleration({0, B_GRAVITY_PULL});
    mGameState.character->setJumpingPower(550);

    std::map<Direction, std::vector<int>> zombieAtlas = {
        {LEFT,  {0,1,2,3,4,5,6,7}},
        {RIGHT, {0,1,2,3,4,5,6,7}},
        {UP,    {0,1,2,3,4,5,6,7}},
        {DOWN,  {0,1,2,3,4,5,6,7}}
    };

    Vector2 zSize = {140,140};
    Vector2 zStart = { mOrigin.x + 300, mOrigin.y - 190 };

    mZombie = new Entity(
        zStart,
        zSize,
        "assets/Zombie/IdleZombie.png",
        ATLAS,
        {8,1},
        zombieAtlas,
        NPC
    );
    mZombie->setAcceleration({0, B_GRAVITY_PULL});
    mZombie->setAIType(FOLLOWER);
    mZombie->setAIState(IDLE);

    mGameState.camera.target = mGameState.character->getPosition();
    mGameState.camera.offset = mOrigin;
    mGameState.camera.zoom = 1.0f;
}

void LevelB::update(float deltaTime)
{
    UpdateMusicStream(mGameState.bgm);

    mGameState.character->update(deltaTime, nullptr, mGameState.map, nullptr, 0);
    mZombie->update(deltaTime, mGameState.character, mGameState.map, nullptr, 0);

    Vector2 camTarget = { mGameState.character->getPosition().x, mOrigin.y };
    panCamera(&mGameState.camera, &camTarget);

    if (mGameState.character->getPosition().y > B_FINISH_TRIGGER)
        mGameState.nextSceneID = 3;
}


void LevelB::render()
{
    ClearBackground(ColorFromHex(mBGColourHexCode));
    mGameState.map->render();
    mZombie->render();
    mGameState.character->render();
}

void LevelB::shutdown()
{
    if (mZombie) { delete mZombie; mZombie = nullptr; }
    if (mGameState.character) { delete mGameState.character; mGameState.character = nullptr; }
    if (mGameState.map) { delete mGameState.map; mGameState.map = nullptr; }

    StopMusicStream(mGameState.bgm);
    UnloadMusicStream(mGameState.bgm);
}
