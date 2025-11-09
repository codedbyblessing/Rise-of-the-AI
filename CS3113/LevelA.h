#ifndef LEVEL_A_H
#define LEVEL_A_H

#include "Scene.h"

const int   A_COLS  = 14;
const int   A_ROWS  = 8;
const float A_TILE  = 75.0f;
const float A_GRAV  = 981.0f;
const float A_FALL_Y = 800.0f; 

class LevelA : public Scene {
public:
    LevelA();
    LevelA(Vector2 origin, const char* backgroundHex);
    ~LevelA();

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;

private:
    unsigned int mGrid[A_COLS * A_ROWS] = {
        4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4,
        4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4,
        4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4,
        4, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 4,
        4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 4,
        4, 2, 2, 2, 0, 0, 0, 2, 2, 2, 3, 3, 3, 4,
        4, 3, 3, 3, 0, 0, 0, 3, 3, 3, 3, 3, 3, 4,
        4, 3, 3, 3, 0, 0, 0, 3, 3, 3, 3, 3, 3, 14   // <- 14 = empty
    };

    Texture2D mBackground {};
};

#endif
