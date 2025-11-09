#include "LevelA.h"

LevelA::LevelA() : Scene({0.0f,0.0f}, "#000000") {}
LevelA::LevelA(Vector2 origin, const char* hex) : Scene(origin, hex) {}
LevelA::~LevelA() { shutdown(); }

void LevelA::initialise() {
    mGameState.nextSceneID = -1;
    mGameState.map = new Map(
        A_COLS, A_ROWS,
        (unsigned int*)mGrid,
        "assets/World_tileset.png",
        A_TILE,
        17, 12,
        mOrigin
    );
    std::map<Direction, std::vector<int>> dudeAtlas = {
        { RIGHT, {0,1,2,3,4,5,6,7,8,9} }
    };

    float sizeRatio = 48.0f / 64.0f;
    mGameState.character = new Entity(
        { mOrigin.x - 300.0f, mOrigin.y - 200.0f },
        { 250.0f * sizeRatio, 250.0f },
        "assets/Dude/IdleDude.png",
        ATLAS,
        { 1, 6 },
        dudeAtlas,
        PLAYER
    );
    mGameState.character->setColliderDimensions({
        mGameState.character->getScale().x / 3.5f,
        mGameState.character->getScale().y / 3.0f
    });
    mGameState.character->setAcceleration({0.0f, A_GRAV});
    mGameState.character->setJumpingPower(550.0f);
    mGameState.character->setFrameSpeed(14);

    std::map<Direction, std::vector<int>> zomAtlas = {
        { RIGHT, {0,1,2,3,4,5,6,7} }
    };
    mGameState.zombie = new Entity(
        { mOrigin.x + 300.0f, mOrigin.y - 200.0f },
        { 160.0f, 160.0f },
        "assets/Zombie/IdleZombie.png",
        ATLAS,
        { 8, 1 },
        zomAtlas,
        ZOMBIE
    );
    mGameState.zombie->setAIType(TRACKER);
    mGameState.zombie->setSpeed(Entity::DEFAULT_SPEED * 0.6f);
    mGameState.zombie->setAcceleration({0.0f, A_GRAV});
    mGameState.zombie->setJumpingPower(450.0f);
    mGameState.zombie->setColliderDimensions({
        mGameState.zombie->getScale().x / 2.0f,
        mGameState.zombie->getScale().y
    });

    mGameState.bgm = LoadMusicStream("assets/GameMusic.mp3");
    SetMusicVolume(mGameState.bgm, 0.5f);
    PlayMusicStream(mGameState.bgm);

   // mGameState.walkingLoop = LoadMusicStream("assets/Walking.mp3");
   // SetMusicVolume(mGameState.walkingLoop, 0.5f);

    //mGameState.attackedSfx = LoadSound("assets/Attacked.mp3");

    mBackground = LoadTexture("assets/background.png");

    mGameState.camera = {0};
    mGameState.camera.target = mGameState.character->getPosition();
    mGameState.camera.offset = mOrigin;
    mGameState.camera.rotation = 0.0f;
    mGameState.camera.zoom = 1.0f;
}

void LevelA::update(float deltaTime) {
    UpdateMusicStream(mGameState.bgm);
   // UpdateMusicStream(mGameState.walkingLoop);

    if (IsKeyDown(KEY_W)) {
        mGameState.character->setTexture("assets/Dude/JumpDude.png");
        mGameState.character->setSpriteSheetDimensions({10,1});
    } else if (IsKeyDown(KEY_A) || IsKeyDown(KEY_D)) {
        mGameState.character->setTexture("assets/Dude/WalkDude.png");
        mGameState.character->setSpriteSheetDimensions({10,1});
    } else {
        mGameState.character->setTexture("assets/Dude/IdleDude.png");
        mGameState.character->setSpriteSheetDimensions({6,1});
    }

    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_D)) {
        //ResumeMusicStream(mGameState.walkingLoop);
    } else {
        //PauseMusicStream(mGameState.walkingLoop);
    }

    if (fabsf(mGameState.zombie->getMovement().x) > 0.01f) {
        mGameState.zombie->setTexture("assets/Zombie/WalkZombie.png");
        mGameState.zombie->setSpriteSheetDimensions({8,1});
    } else {
        mGameState.zombie->setTexture("assets/Zombie/IdleZombie.png");
        mGameState.zombie->setSpriteSheetDimensions({8,1});
    }

    mGameState.character->update(deltaTime, nullptr, mGameState.map, nullptr, 0);
    mGameState.zombie->update(deltaTime, mGameState.character, mGameState.map, nullptr, 0);

    if (mGameState.character->intersects(mGameState.zombie)) {
        //PlaySound(mGameState.attackedSfx);
        mGameState.playerLives -= 1;
        mGameState.nextSceneID = (mGameState.playerLives <= 0) ? 0 : 1;
        return;
    }

    Vector2 camTarget = { mGameState.character->getPosition().x, mOrigin.y };
    panCamera(&mGameState.camera, &camTarget);

    if (mGameState.character->getPosition().y > A_FALL_Y) {
        mGameState.nextSceneID = 2; 
        return;
    }
}
void LevelA::render() {
    ClearBackground(ColorFromHex(mBGColourHexCode));
    if (mBackground.id) {
        Rectangle src = {0,0,(float)mBackground.width,(float)mBackground.height};
        Rectangle dst = { mGameState.camera.target.x, mGameState.camera.target.y, 2000.0f, 1200.0f };
        Vector2   org = { dst.width/2.0f, dst.height/2.0f };
        DrawTexturePro(mBackground, src, dst, org, 0.0f, WHITE);
    }

    mGameState.map->render();
    mGameState.character->render();
    mGameState.zombie->render();


    EndMode2D();
    DrawText(TextFormat("Lives: %d", mGameState.playerLives), 20, 20, 24, WHITE);
    BeginMode2D(mGameState.camera);
}

void LevelA::shutdown() {
    if (mGameState.character) { delete mGameState.character; mGameState.character = nullptr; }
    if (mGameState.zombie)    { delete mGameState.zombie;    mGameState.zombie    = nullptr; }
    if (mGameState.map)       { delete mGameState.map;       mGameState.map       = nullptr; }

    StopMusicStream(mGameState.bgm);
    UnloadMusicStream(mGameState.bgm);

   // StopMusicStream(mGameState.walkingLoop);
   // UnloadMusicStream(mGameState.walkingLoop);

   // UnloadSound(mGameState.attackedSfx);

    if (mBackground.id) UnloadTexture(mBackground);
}
