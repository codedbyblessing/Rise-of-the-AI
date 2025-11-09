#include "Menu.h"

MenuScene::MenuScene() : Scene({0.0f,0.0f}, "#000000") {}
MenuScene::MenuScene(Vector2 origin, const char* bgHex) : Scene(origin, bgHex) {}
MenuScene::~MenuScene() { shutdown(); }

void MenuScene::initialise() {
    mGameState.nextSceneID = -1;

    // Menu music
    mGameState.bgm = LoadMusicStream("assets/LoadingMusic.mp3");
    SetMusicVolume(mGameState.bgm, 0.6f);
    PlayMusicStream(mGameState.bgm);

    // Reset lives when returning to menu
    mGameState.playerLives = 3;
    mGameState.gameLost = false;
    mGameState.gameWon  = false;
}

void MenuScene::update(float deltaTime) {
    UpdateMusicStream(mGameState.bgm);

    if (IsKeyPressed(KEY_ENTER)) { //loads levelA
        mGameState.nextSceneID = 1;
    }
}

void MenuScene::render() {
    ClearBackground(BLACK);
    const char* title = "Rise of the AI";
    const char* prompt = "Press ENTER to start";
    DrawText(title,  80,  180, 60, WHITE);
    DrawText(prompt, 80,  280, 30, WHITE);
    DrawText("WASD to move/jump, Q to quit\n You have THREE Lives", 80, 330, 20, GRAY);
}

void MenuScene::shutdown() {
    StopMusicStream(mGameState.bgm);
    UnloadMusicStream(mGameState.bgm);
}
