#ifndef LEVEL_B_H
#define LEVEL_B_H

#include "Scene.h"
#include "raylib.h"
#include <map>
#include <vector>

class LevelB : public Scene {
public:
    static const int B_COLUMNS = 16;
    static const int B_ROWS    = 9;

    static const float B_TILE_SIZE;
    static const float B_GRAVITY_PULL;
    static const float B_FINISH_TRIGGER;

    LevelB();
    LevelB(Vector2 origin, const char* bgHex);
    ~LevelB();

    void initialise() override;
    //i need to override because each level has its own version
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;

private:
    // Tile array
    unsigned int mGridB[B_COLUMNS * B_ROWS];

    // Zombie pointer
    Entity* mZombie;
};

#endif
