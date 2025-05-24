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

    zoomFactorPtr = map->getZoomFactorPtr();
}

Human::~Human() {
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