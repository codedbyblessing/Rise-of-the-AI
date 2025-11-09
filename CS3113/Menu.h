#ifndef MENU_H
#define MENU_H

#include "Scene.h"

class MenuScene : public Scene {
public:
    MenuScene();
    MenuScene(Vector2 origin, const char* bgHex);
    ~MenuScene();

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif
