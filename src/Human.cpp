#include "Human.h"
#include <iostream>
#include "Map.h"
#include "Window.h"

SDL_Texture* Human::loadTexture(const char* filePath, SDL_Renderer* renderer) {
    SDL_Texture* texture = IMG_LoadTexture(renderer, filePath);
    if (!texture) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load texture: %s", IMG_GetError());
        throw std::runtime_error("Failed to load texture");
    }
    return texture;
}

Human::Human(float x, float y, Window* window, Map* map)
    : x(x), y(y), window(window), map(map), health(100) {
    dx = 0.0f;
    dy = 0.0f;
    speed = 100;
    currentFrame = 0;
    timeSinceLastFrame = 0.0f;

    numFrames = 7;
    frameWidth = 32;
    frameHeight = 32;

    frameTime = 0.1f;
    isWalking = false;
    walkingDirection = 0;
    isPlayer = false;
    renderer = window->getRenderer();
    windowWidthPtr = window->getWidthPtr();
    windowHeightPtr = window->getHeightPtr();

    pathfinder = new Pathfinder(map);

    // humanTexture = loadTexture("assets/Characters/Human.png", renderer);
    // zombieTexture = loadTexture("assets/Characters/Zombie.png", renderer);

    // textureFlip = SDL_FLIP_NONE;
    zoomFactorPtr = map->getZoomFactorPtr();
}

Human::~Human() {
    // SDL_DestroyTexture(humanTexture);
    // SDL_DestroyTexture(zombieTexture);
    delete pathfinder;
}


Pathfinder* Human::getPathfinder(){
    return pathfinder;
}

void Human::move(float dx, float dy) {
    int mapWidth = map->getMapWidth();
    int mapHeight = map->getMapHeight();
    int tileSize = map->getTileSize();

    float oldX = x;
    float oldY = y;

    x += dx / *zoomFactorPtr;
    y += dy / *zoomFactorPtr;

    if (x < 0 || x > mapWidth * tileSize) {
        x = oldX;
    }
    if (y < 0 || y > mapHeight * tileSize) {
        y = oldY;
    }

    Tile* tilePtr = map->getTileAtPixelPtr(x, y);
    if (!tilePtr->getIsWalkable()) {
        x = oldX;
        y = oldY;
    }
}

float Human::getSpeed() const {
    return speed;
}


std::pair<int, int> Human::getWindowSize() {
    return { *windowWidthPtr, *windowHeightPtr };
}


// void Human::draw(int cameraX, int cameraY) {
//     int row;

//     if (isWalking) {
//         row = 1;
//         numFrames = 4;
//     } else {
//         if (!isPlayer) {
//             row = 2;
//             numFrames = 4;
//         } else {
//             row = 0;
//             numFrames = 7;
//         }
//     }

//     if (walkingDirection == 4) {
//         textureFlip = SDL_FLIP_HORIZONTAL;
//     } else if (walkingDirection == 2) {
//         textureFlip = SDL_FLIP_NONE;
//     }

//     float zoomFactor = map->getZoomFactor();
//     int drawX = static_cast<int>((x - cameraX) * zoomFactor + (*windowWidthPtr / 2));
//     int drawY = static_cast<int>((y - cameraY) * zoomFactor + (*windowHeightPtr / 2));

//     if (drawY > *windowHeightPtr) {
//         return;
//     } else {
//         int zoomedFrameWidth = static_cast<int>(frameWidth * zoomFactor);
//         int zoomedFrameHeight = static_cast<int>(frameHeight * zoomFactor);
//         SDL_Rect srcRect = { currentFrame * frameWidth, row * frameHeight, frameWidth, frameHeight };
//         SDL_Rect destRect = { drawX - zoomedFrameWidth/2, drawY-zoomedFrameHeight/2, zoomedFrameWidth, zoomedFrameHeight };
//         SDL_Texture* texture = isPlayer ? humanTexture : zombieTexture;
//         SDL_RenderCopyEx(renderer, texture, &srcRect, &destRect, 0, NULL, textureFlip);
//     }
    
// }

bool Human::getIsWalking() {
    return isWalking;
}


int Human::getWalkingDirection() {
    return walkingDirection;
}

int Human::getCurrentFrame() {
    return currentFrame;
}

void Human::setIsWalking(bool isWalking) {
    this->isWalking = isWalking;
}


void Human::setWalkingDirection(int walkingDirection) {
    this->walkingDirection = walkingDirection;
}

void Human::setCurrentFrame(int currentFrame) {
    this->currentFrame = currentFrame;
}


std::pair<float, float> Human::getCoordinates() const {
    return {x, y};
}

Map* Human::getMap() {
    return map;
}

void Human::updateAnimation(float deltaTime){
    timeSinceLastFrame += deltaTime;
    if (timeSinceLastFrame >= frameTime) {
        currentFrame = (currentFrame + 1) % numFrames;
        timeSinceLastFrame = 0.0f;
    }
}

void Human::updateHuman(float deltaTime) {
    updateAnimation(deltaTime);
}

float* Human::getX() {
    return &x;
}

float* Human::getY() {
    return &y;
}

void Human::setIsPlayer(bool isPlayer) {
    this->isPlayer = isPlayer;
}

void Human::setSpeed(float newSpeed) {
    speed = newSpeed;
}

void Human::takeDamage(int damage) {
    health -= damage;
}

int Human::getHealth() {
    return health;
}

void Human::setHealth(int newHealth) {
    health = newHealth;
}

float Human::getZoomFactor() {
    return *zoomFactorPtr;
}

SDL_Renderer* Human::getRenderer() {
    return renderer;
}

std::pair<int, int> Human::getFrameSize() {
    return { frameWidth, frameHeight };
}

void Human::setNumFrames(int numFrames) {
    this->numFrames = numFrames;
}