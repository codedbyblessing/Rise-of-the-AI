#include "Entity.h"

const float Entity::Y_COLLISION_THRESHOLD = 0.5f;

Entity::Entity()
: mScale{(float)DEFAULT_SIZE, (float)DEFAULT_SIZE},
  mColliderDimensions{(float)DEFAULT_SIZE, (float)DEFAULT_SIZE},
  mTexture{}, mTextureType(SINGLE),
  mDirection(RIGHT), mFrameSpeed(DEFAULT_FRAME_SPEED), mSpeed(DEFAULT_SPEED),
  mEntityType(NONE), mAIType(WANDERER), mAIState(IDLE) {}

Entity::Entity(Vector2 position, Vector2 scale, const char* textureFilepath, EntityType entityType)
: mPosition{position}, mScale{scale}, mColliderDimensions{scale}, mTexture{LoadTexture(textureFilepath)},
  mTextureType(SINGLE), mDirection(RIGHT), mFrameSpeed(DEFAULT_FRAME_SPEED), mSpeed(DEFAULT_SPEED),
  mEntityType(entityType), mAIType(WANDERER), mAIState(IDLE) {}

Entity::Entity(Vector2 position, Vector2 scale, const char* textureFilepath, TextureType textureType,
               Vector2 spriteSheetDimensions, std::map<Direction, std::vector<int>> animationAtlas, EntityType entityType)
: mPosition{position}, mScale{scale}, mColliderDimensions{scale}, mTexture{LoadTexture(textureFilepath)},
  mTextureType(textureType), mSpriteSheetDimensions{spriteSheetDimensions}, mAnimationAtlas{animationAtlas},
  mAnimationIndices(animationAtlas.begin()->second), mDirection(RIGHT), mFrameSpeed(DEFAULT_FRAME_SPEED),
  mSpeed(DEFAULT_SPEED), mEntityType(entityType), mAIType(WANDERER), mAIState(IDLE) {}

Entity::~Entity() { if (mTexture.id) UnloadTexture(mTexture); }

void Entity::setTexture(const char* textureFilepath) {
    if (mTexture.id) UnloadTexture(mTexture);
    mTexture = LoadTexture(textureFilepath);
    
}

void Entity::setDirection(Direction d) {
    mDirection = d;
    if (mTextureType == ATLAS && !mAnimationAtlas.empty()) {
        auto it = mAnimationAtlas.find(mDirection);
        if (it != mAnimationAtlas.end()) mAnimationIndices = it->second;
    }
}

void Entity::normaliseMovement() { Normalise(&mMovement); }

void Entity::jump() {
    mIsJumping = true;
}

void Entity::animate(float deltaTime) {
    if (mAnimationAtlas.empty()) return;
    mAnimationTime += deltaTime;
    float framesPerSecond = 1.0f / (float)mFrameSpeed;

    if (mAnimationTime >= framesPerSecond) {
        mAnimationTime = 0.0f;
        if (!mAnimationIndices.empty()) {
            mCurrentFrameIndex++;
            mCurrentFrameIndex %= (int)mAnimationIndices.size();
        }
    }
}

bool Entity::isColliding(Entity* other) const {
    if (!other || !other->isActive() || other == this) return false;
    float xDistance = fabsf(mPosition.x - other->mPosition.x) - ((mColliderDimensions.x + other->mColliderDimensions.x) / 2.0f);
    float yDistance = fabsf(mPosition.y - other->mPosition.y) - ((mColliderDimensions.y + other->mColliderDimensions.y) / 2.0f);
    return (xDistance < 0.0f && yDistance < 0.0f);
}

bool Entity::intersects(const Entity* other) const {
    if (!other) return false;
    float xDistance = fabsf(mPosition.x - other->getPosition().x) - ((mColliderDimensions.x + other->getColliderDimensions().x) / 2.0f);
    float yDistance = fabsf(mPosition.y - other->getPosition().y) - ((mColliderDimensions.y + other->getColliderDimensions().y) / 2.0f);
    return (xDistance < 0.0f && yDistance < 0.0f);
}

void Entity::AIWander() {
    float t = (float)GetTime();
    float s = sinf(t);
    if (s >= 0) moveRight(); else moveLeft();
}

void Entity::AIFollow(Entity* target) {
    if (!target) return;
    float dx = target->getPosition().x - mPosition.x;
    if (dx < -5.0f) moveLeft();
    else if (dx > 5.0f) moveRight();
}

void Entity::AIFly(Entity* target) {
    if (!target) return;
    Vector2 dir = Vector2Subtract(target->getPosition(), mPosition);
    dir = Vector2Normalize(dir);
    mMovement = dir;
}

void Entity::AITrack(Entity* target) {
 
    if (!target) { AIWander(); return; }

    float dist = Vector2Distance(mPosition, target->getPosition());
    if (dist > 300.0f) {
        AIWander();
    } else {
        AIFollow(target);
        float dy = target->getPosition().y - mPosition.y;
        if (dy < -50.0f && mIsCollidingBottom) {
            mIsJumping = true;
        }
    }
}

void Entity::AIActivate(Entity* target) {
    switch (mAIType) {
        case WANDERER: AIWander(); break;
        case FOLLOWER: AIFollow(target); break;
        case FLYER:    AIFly(target);    break;
        case TRACKER:  AITrack(target);  break;
        default: break;
    }
}

void Entity::checkCollisionY(Entity* collidableEntities, int count) {
    if (!collidableEntities || count <= 0) return;
    for (int i = 0; i < count; i++) {
        Entity* e = &collidableEntities[i];
        if (!e->isActive()) continue;
        if (!isColliding(e)) continue;

        float yDist = fabsf(mPosition.y - e->getPosition().y);
        float overlap = fabsf(yDist - (mColliderDimensions.y / 2.0f) - (e->getColliderDimensions().y / 2.0f));

        if (mVelocity.y > 0.0f) { 
            mPosition.y -= overlap;
            mVelocity.y  = 0.0f;
            mIsCollidingBottom = true;
        } else if (mVelocity.y < 0.0f) { 
            mPosition.y += overlap;
            mVelocity.y  = 0.0f;
            mIsCollidingTop = true;
        }
    }
}

void Entity::checkCollisionX(Entity* collidableEntities, int count) {
    if (!collidableEntities || count <= 0) return;
    for (int i = 0; i < count; i++) {
        Entity* e = &collidableEntities[i];
        if (!e->isActive()) continue;
        if (!isColliding(e)) continue;

        float yDist = fabsf(mPosition.y - e->getPosition().y);
        float yOv   = fabsf(yDist - (mColliderDimensions.y / 2.0f) - (e->getColliderDimensions().y / 2.0f));
        if (yOv < Y_COLLISION_THRESHOLD) continue;

        float xDist = fabsf(mPosition.x - e->getPosition().x);
        float xOv   = fabsf(xDist - (mColliderDimensions.x / 2.0f) - (e->getColliderDimensions().x / 2.0f));

        if (mVelocity.x > 0.0f) { mPosition.x -= xOv; mVelocity.x = 0.0f; mIsCollidingRight = true; }
        else if (mVelocity.x < 0.0f) { mPosition.x += xOv; mVelocity.x = 0.0f; mIsCollidingLeft = true; }
    }
}

void Entity::checkCollisionY(Map* map) {(void)map;}
void Entity::checkCollisionX(Map* map) {(void)map;}

void Entity::update(float deltaTime, Entity* player, Map* map, Entity* collidables, int count) {
    if (mEntityStatus == INACTIVE) return;

    if (mEntityType == NPC || mEntityType == ZOMBIE) AIActivate(player);

    resetColliderFlags();

    mVelocity.x = mMovement.x * (float)mSpeed;
    mVelocity.x += mAcceleration.x * deltaTime;
    mVelocity.y += mAcceleration.y * deltaTime;

    if (mIsJumping) {
        mIsJumping = false;
        mVelocity.y = -mJumpingPower;
    }


    mPosition.y += mVelocity.y * deltaTime;
    checkCollisionY(collidables, count);
    checkCollisionY(map);


    mPosition.x += mVelocity.x * deltaTime;
    checkCollisionX(collidables, count);
    checkCollisionX(map);

    if (mTextureType == ATLAS && GetLength(mMovement) > 0.05f && mIsCollidingBottom) {
        animate(deltaTime);
    }
}

void Entity::render() {
    if (!isActive()) return;

    Rectangle src;
    if (mTextureType == SINGLE) {
        src = {0,0,(float)mTexture.width,(float)mTexture.height};
    } else {
 
        int frame = mAnimationIndices.empty() ? 0 : mAnimationIndices[mCurrentFrameIndex];
        src = getUVRectangle(&mTexture, frame, mSpriteSheetDimensions.x, mSpriteSheetDimensions.y);
    }

    Rectangle dst = { mPosition.x, mPosition.y, mScale.x, mScale.y };
    Vector2   org = { mScale.x / 2.0f, mScale.y / 2.0f };

    //DrawTexturePro(mTexture, src, dst, org, mAngle, WHITE);
}

void Entity::displayCollider() {
    Rectangle r {
        mPosition.x - mColliderDimensions.x/2.0f,
        mPosition.y - mColliderDimensions.y/2.0f,
        mColliderDimensions.x,
        mColliderDimensions.y
    };
}


