#include "Zombie.h"
#include <random>
#include <iostream>

Zombie::Zombie(float x, float y, Window* window, Map* map)
    : Human(x, y, window, map), damage(10), timeSinceLastAttack(0), viewRange(200), textureFlip(SDL_FLIP_NONE) {
    setSpeed(50);
    loadAnimations();
}

void Zombie::moveTo(float destinationX, float destinationY) {
    float x = *getX();
    float y = *getY();
    currentPath = getPathfinder()->findPath(x, y, destinationX, destinationY);
}

void Zombie::clearPath() {
    currentPath.clear();
}

bool Zombie::isPathEmpty() {
    return currentPath.empty();
}

void Zombie::update(float deltaTime) {
    updateAnimation(deltaTime);
    if (target) {
        timeSinceLastAttack += deltaTime;
        float targetX = *target->getX();
        float targetY = *target->getY();

        float distance = std::sqrt((targetX - *getX()) * (targetX - *getX()) + (targetY - *getY()) * (targetY - *getY()));
        if (distance > viewRange) {
            clearPath();
            if (getIsWalking()) {
                setIsWalking(false);
                switchTexture(); // Switch to idle texture when stopping movement
            }
            return;
        }
        
        // Recalculate path every second or when the path is empty
        static float timeSinceLastPathfinding = 0;
        timeSinceLastPathfinding += deltaTime;
        if (timeSinceLastPathfinding >= 0.5f || isPathEmpty()) {
            clearPath();
            moveTo(targetX, targetY);
            timeSinceLastPathfinding = 0;
        }

        float& x = *getX();
        float& y = *getY();

        if (!isPathEmpty()) {
            
            float nextX = currentPath.front().first;
            float nextY = currentPath.front().second;
            
            float speed = getSpeed();
            
            float dx = nextX - x;
            float dy = nextY - y;
            float distance = std::sqrt(dx*dx + dy*dy);
            
            if (distance < speed * deltaTime) {
                // Reached the next point
                x = nextX;
                y = nextY;
                currentPath.erase(currentPath.begin());
            } else {
                // Move towards the next point
                float ratio = (speed * deltaTime) / distance;
                x += dx * ratio;
                y += dy * ratio;
            }

            if (!getIsWalking()) {
                setIsWalking(true);
                switchTexture(); // Switch to walking texture when starting movement
            }   
            
            if (std::abs(dx) > std::abs(dy)) {
                setWalkingDirection((dx > 0) ? 2 : 4);  // Right : Left
            } else {
                setWalkingDirection((dy > 0) ? 3 : 1);  // Down : Up
            }
            
        } else {
            if (getIsWalking()) {
                setIsWalking(false);
                switchTexture(); // Switch to idle texture when stopping movement
            }
        }
        float distanceToTarget = std::sqrt((targetX - x) * (targetX - x) + (targetY - y) * (targetY - y));
        if (distanceToTarget < 10 && timeSinceLastAttack >= 1) {
            target->takeDamage(damage);
            timeSinceLastAttack = 0;
        }
    }
}

void Zombie::setTarget(Player* target) {
    this->target = target;
}

bool Zombie::isDead() {
    return getHealth() <= 0;
}

void Zombie::loadAnimations(){
    SDL_Renderer* renderer = getRenderer();
    SDL_Texture* zombieTexture = loadTexture("assets/Characters/Zombie.png", renderer);

    idleTextures.clear();
    walkTextures.clear();
    sleepTextures.clear();
    attackTextures.clear();
    
    int row, numFrames;
    int frameWidth, frameHeight;
    std::tie(frameWidth, frameHeight) = getFrameSize();

    // Load idle textures
    row = 0;
    numFrames = 7;
    for (int i = 0; i < numFrames; i++) {
        SDL_Texture* texture = SDL_CreateTexture(
            renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, frameWidth, frameHeight
        );
        SDL_SetRenderTarget(renderer, texture);
        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
        SDL_Rect srcRect = { i * frameWidth, row * frameHeight, frameWidth, frameHeight };
        SDL_Rect dstRect = { 0, 0, frameWidth, frameHeight };
        SDL_RenderCopy(renderer, zombieTexture, &srcRect, &dstRect);
        SDL_SetRenderTarget(renderer, nullptr);
        idleTextures.push_back(texture);
    }

    // Load walk textures
    row = 1;
    numFrames = 4;
    for (int i = 0; i < numFrames; i++) {
        SDL_Texture* texture = SDL_CreateTexture(
            renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, frameWidth, frameHeight
        );
        SDL_SetRenderTarget(renderer, texture);
        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

        SDL_Rect srcRect = { i * frameWidth, row * frameHeight, frameWidth, frameHeight };
        SDL_Rect dstRect = { 0, 0, frameWidth, frameHeight };
        SDL_RenderCopy(renderer, zombieTexture, &srcRect, &dstRect);
        SDL_SetRenderTarget(renderer, nullptr);
        walkTextures.push_back(texture);
    }

    // Load sleep textures
    row = 2;
    // numFrames = 4;
    for (int i = 0; i < numFrames; i++) {
        SDL_Texture* texture = SDL_CreateTexture(
            renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, frameWidth, frameHeight
        );
        SDL_SetRenderTarget(renderer, texture);
        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

        SDL_Rect srcRect = { i * frameWidth, row * frameHeight, frameWidth, frameHeight };
        SDL_Rect dstRect = { 0, 0, frameWidth, frameHeight };
        SDL_RenderCopy(renderer, zombieTexture, &srcRect, &dstRect);
        SDL_SetRenderTarget(renderer, nullptr);
        sleepTextures.push_back(texture);
    }

    // Load attack textures
    row = 3;
    // numFrames = 4;
    for (int i = 0; i < numFrames; i++) {
        SDL_Texture* texture = SDL_CreateTexture(
            renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, frameWidth, frameHeight
        );
        SDL_SetRenderTarget(renderer, texture);
        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

        SDL_Rect srcRect = { i * frameWidth, row * frameHeight, frameWidth, frameHeight };
        SDL_Rect dstRect = { 0, 0, frameWidth, frameHeight };
        SDL_RenderCopy(renderer, zombieTexture, &srcRect, &dstRect);
        SDL_SetRenderTarget(renderer, nullptr);
        attackTextures.push_back(texture);
    }

    SDL_DestroyTexture(zombieTexture);
    currentTextures = &sleepTextures;
    setNumFrames(currentTextures->size());
}

void Zombie::switchTexture() {
    if (getIsWalking()) {
        currentTextures = &walkTextures;
    } else {
        currentTextures = &sleepTextures;
    }
    setCurrentFrame(0);
    setNumFrames(currentTextures->size());
}

void Zombie::draw(int cameraX, int cameraY){
    int walkingDirection = getWalkingDirection();
    SDL_Renderer* renderer = getRenderer();
    if (walkingDirection == 4) {
        textureFlip = SDL_FLIP_HORIZONTAL;
    } else if (walkingDirection == 2) {
        textureFlip = SDL_FLIP_NONE;
    }

    float zoomFactor = getMap()->getZoomFactor();
    int windowWidth, windowHeight;
    std::tie(windowWidth, windowHeight) = getWindowSize();

    float x, y;
    std::tie(x, y) = getCoordinates();

    int frameWidth, frameHeight;
    std::tie(frameWidth, frameHeight) = getFrameSize();

    int zoomedFrameWidth = static_cast<int>(frameWidth * zoomFactor);
    int zoomedFrameHeight = static_cast<int>(frameHeight * zoomFactor);

    int drawX = static_cast<int>((x - cameraX) * zoomFactor + (windowWidth / 2));
    int drawY = static_cast<int>((y - cameraY) * zoomFactor + (windowHeight / 2));

    SDL_Texture* texture = currentTextures->at(getCurrentFrame());
    SDL_Rect destRect = { drawX - zoomedFrameWidth/2, drawY-zoomedFrameHeight/2, zoomedFrameWidth, zoomedFrameHeight };
    SDL_RenderCopyEx(renderer, texture, nullptr, &destRect, 0, NULL, textureFlip);
}