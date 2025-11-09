#include "Scene.h"

Scene::Scene()
: mOrigin{0.0f, 0.0f},
  mBGColourHexCode{"#000000"}
{
    mGameState.nextSceneID = -1;
    mGameState.playerLives = 3;
    mGameState.gameWon     = false;
    mGameState.gameLost    = false;

    mGameState.camera.target   = mOrigin;     
    mGameState.camera.offset   = mOrigin;     
    mGameState.camera.rotation = 0.0f;
    mGameState.camera.zoom     = 1.0f;
}


Scene::Scene(Vector2 origin, const char* bgHexCode)
: mOrigin{origin},
  mBGColourHexCode{bgHexCode}
{
 
    mGameState.nextSceneID = -1;
    mGameState.playerLives = 3;
    mGameState.gameWon     = false;
    mGameState.gameLost    = false;

    mGameState.camera.target   = origin;    
    mGameState.camera.offset   = origin;      
    mGameState.camera.rotation = 0.0f;
    mGameState.camera.zoom     = 1.0f;
}
