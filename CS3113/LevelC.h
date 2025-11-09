#ifndef LEVEL_C_H
#define LEVEL_C_H

#include "Scene.h"
#include "raylib.h"

enum { C_WIDTH = 18, C_HEIGHT = 10 };

class LevelC : public Scene {
public:
    static const float C_TILE_UNIT;
    static const float C_FALL_FORCE;
    static const float C_EXIT_LIMIT;

    LevelC();
    LevelC(Vector2 origin, const char* hexBackground);
    ~LevelC();

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;

private:
    unsigned int mTileMatrixC[C_WIDTH * C_HEIGHT] = {
        4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
        4,0,0,2,2,0,0,0,3,3,3,0,0,2,2,0,0,4,
        4,0,0,2,0,0,0,0,3,0,3,0,0,0,2,0,0,4,
        4,0,0,2,0,0,0,0,3,0,3,0,0,0,2,0,0,4,
        4,0,0,0,0,0,3,3,3,3,3,3,0,0,0,0,0,4,
        4,0,3,3,0,0,3,0,0,0,0,3,0,0,3,3,0,4,
        4,0,3,3,0,0,3,0,0,0,0,3,0,0,3,3,0,4,
        4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,
        4,0,0,0,0,2,2,2,2,2,2,2,2,0,0,0,0,4,
        4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4
    };
};

#endif // LEVEL_C_H
