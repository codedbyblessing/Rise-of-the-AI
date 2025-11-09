#ifndef ENTITY_H
#define ENTITY_H

#include "cs3113.h" 
#include "Map.h"
#include <map>
#include <vector>

enum Direction    { LEFT, UP, RIGHT, DOWN };
enum EntityStatus { ACTIVE, INACTIVE };
enum EntityType   { PLAYER, BLOCK, PLATFORM, NPC, ZOMBIE, NONE };
enum AIType       { WANDERER, FOLLOWER, FLYER, TRACKER };
enum AIState      { IDLE, WALKING, FOLLOWING };

class Entity {
private:
    Vector2    mPosition {0,0};
    Vector2    mMovement {0,0};
    Vector2    mVelocity {0,0};
    Vector2    mAcceleration {0,0};

    Vector2    mScale {0,0};
    Vector2    mColliderDimensions {0,0};

    Texture2D  mTexture {};
    TextureType mTextureType = SINGLE;
    Vector2    mSpriteSheetDimensions {1,1};

    std::map<Direction, std::vector<int>> mAnimationAtlas;
    std::vector<int> mAnimationIndices;
    Direction  mDirection = RIGHT;
    int        mFrameSpeed = 14;

    int        mCurrentFrameIndex = 0;
    float      mAnimationTime = 0.0f;

    bool       mIsJumping = false;
    float      mJumpingPower = 0.0f;

    int        mSpeed = 200;
    float      mAngle = 0.0f;

    bool       mIsCollidingTop    = false;
    bool       mIsCollidingBottom = false;
    bool       mIsCollidingRight  = false;
    bool       mIsCollidingLeft   = false;

    EntityStatus mEntityStatus = ACTIVE;
    EntityType   mEntityType   = NONE;

    AIType    mAIType  = WANDERER;
    AIState   mAIState = IDLE;

    bool  isColliding(Entity* other) const;

    // Map collision helpers (implemented in .cpp)
    void checkCollisionY(Entity* collidableEntities, int collisionCheckCount);
    void checkCollisionX(Entity* collidableEntities, int collisionCheckCount);
    void checkCollisionY(Map* map);
    void checkCollisionX(Map* map);

    void animate(float deltaTime);
    void AIActivate(Entity* target);
    void AIWander();
    void AIFollow(Entity* target);
    void AIFly(Entity* target);
    void AITrack(Entity* target); // wander + track + jump if needed

    void resetColliderFlags() {
        mIsCollidingTop = mIsCollidingBottom = mIsCollidingRight = mIsCollidingLeft = false;
    }

public:
    static const int   DEFAULT_SIZE        = 250;
    static const int   DEFAULT_SPEED       = 200;
    static const int   DEFAULT_FRAME_SPEED = 14;
    static const float Y_COLLISION_THRESHOLD;

    Entity();
    Entity(Vector2 position, Vector2 scale, const char* textureFilepath, EntityType entityType);
    Entity(Vector2 position, Vector2 scale, const char* textureFilepath, TextureType textureType,
           Vector2 spriteSheetDimensions, std::map<Direction, std::vector<int>> animationAtlas, EntityType entityType);
    ~Entity();

    void update(float deltaTime, Entity* player, Map* map, Entity* collidableEntities, int collisionCheckCount);
    void render();
    void displayCollider();

    void normaliseMovement();
    void jump();

    // movement intent
    void moveUp()    { mMovement.y = -1; mDirection = UP;    }
    void moveDown()  { mMovement.y =  1; mDirection = DOWN;  }
    void moveLeft()  { mMovement.x = -1; mDirection = LEFT;  }
    void moveRight() { mMovement.x =  1; mDirection = RIGHT; }
    void resetMovement() { mMovement = {0,0}; }

    void activate()   { mEntityStatus = ACTIVE; }
    void deactivate() { mEntityStatus = INACTIVE; }

    // Public overlap query for level logic
    bool intersects(const Entity* other) const;

    // Getters
    bool     isActive()           const { return mEntityStatus == ACTIVE; }
    bool     isCollidingTop()     const { return mIsCollidingTop; }
    bool     isCollidingBottom()  const { return mIsCollidingBottom; }
    Vector2  getPosition()        const { return mPosition; }
    Vector2  getMovement()        const { return mMovement; }
    Vector2  getVelocity()        const { return mVelocity; }
    Vector2  getScale()           const { return mScale; }
    Vector2  getColliderDimensions() const { return mColliderDimensions; }
    Direction getDirection()      const { return mDirection; }
    EntityType getEntityType()    const { return mEntityType; }
    AIType     getAIType()        const { return mAIType; }
    AIState    getAIState()       const { return mAIState; }

    // Setters
    void setPosition(Vector2 p)                 { mPosition = p; }
    void setAcceleration(Vector2 a)             { mAcceleration = a; }
    void setScale(Vector2 s)                    { mScale = s; }
    void setColliderDimensions(Vector2 d)       { mColliderDimensions = d; }
    void setSpriteSheetDimensions(Vector2 d)    { mSpriteSheetDimensions = d; }
    void setSpeed(int s)                        { mSpeed = s; }
    void setFrameSpeed(int s)                   { mFrameSpeed = s; }
    void setJumpingPower(float jp)              { mJumpingPower = jp; }
    void setAngle(float ang)                    { mAngle = ang; }
    void setEntityType(EntityType t)            { mEntityType = t; }
    void setAIType(AIType t)                    { mAIType = t; }
    void setAIState(AIState s)                  { mAIState = s; }
    void setDirection(Direction d);
    void setTexture(const char* textureFilepath);
};

#endif
