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
    frameWidth = 48;
    frameHeight = 30;
    frameTime = 0.1f;
    isWalking = false;
    walkingDirection = 0;
    isPlayer = false;
    renderer = window->getRenderer();
    windowWidthPtr = window->getWidthPtr();
    windowHeightPtr = window->getHeightPtr();

    pathfinder = new Pathfinder(map);

    texture = loadTexture("assets/Character-and-Zombie.png", renderer);
    textureFlip = SDL_FLIP_NONE;
}

Human::~Human() {
    SDL_DestroyTexture(texture);
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

    x += dx;
    y += dy;

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

void Human::draw(int cameraX, int cameraY) {
    int textureWidth;
    SDL_QueryTexture(texture, NULL, NULL, &textureWidth, NULL);

    int row;

    if (isPlayer){
        if (isWalking) {
            row = 3;
            numFrames = 7;
        } else {
            row = 1;
            numFrames = 14;
        }
    } else {
        row = 4;
        numFrames = 7;
    }

    if (walkingDirection == 4) {
        textureFlip = SDL_FLIP_HORIZONTAL;
    } else if (walkingDirection == 2) {
        textureFlip = SDL_FLIP_NONE;
    }

    int drawX = (x - cameraX) + *windowWidthPtr / 2;
    int drawY = (y - cameraY) + *windowHeightPtr / 2;
    if (drawY > *windowHeightPtr) {
        return;
    } else {
        SDL_Rect srcRect = { currentFrame * frameWidth, row * frameHeight, frameWidth, frameHeight };
        SDL_Rect destRect = { static_cast<int>(drawX) - frameWidth / 2, static_cast<int>(drawY) - frameHeight / 2, frameWidth, frameHeight };
        SDL_RenderCopyEx(renderer, texture, &srcRect, &destRect, 0, NULL, textureFlip);
    }
    
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