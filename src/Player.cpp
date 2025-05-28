#include "Player.h"
#include "Zombie.h"
#include "Light.h"
#include "Window.h"
#include <iostream>

Player::Player(float x, float y, Window* window, Map* map)
    : Human(x, y, window, map), score(0), textureFlip(SDL_FLIP_NONE) {
    setIsPlayer(true);
    gun = new Gun(window, this, 20, 200, 20, 10);
    tpStart = std::chrono::steady_clock::now();
    light = new Light(&x, &y, 100, 10, 200, window);
    window->addLight(light);
    loadAnimations();
}

void Player::keyDown(SDL_Keycode key) {
    Map* mapPtr = getMap();
    if (key == SDLK_p) {
        //talk();
        mapPtr->setZoomFactor(mapPtr->getZoomFactor() * 2.0f);
    } else if (key == SDLK_m) {
        mapPtr->setZoomFactor(mapPtr->getZoomFactor() / 2.0f);
    }
}

void Player::mouseUpdate(int mouseX, int mouseY) {
    int playerX, playerY;
    std::tie(playerX, playerY) = getCoordinates();

    int windowWidth, windowHeight;
    std::tie(windowWidth, windowHeight) = getWindowSize();

    int worldMouseX = mouseX + (playerX - windowWidth / 2);
    int worldMouseY = mouseY + (playerY - windowHeight / 2);

    // Calculate direction vector from player to mouse
    float directionX = worldMouseX - playerX;
    float directionY = worldMouseY - playerY;

    gun->shoot(directionX, directionY);
}

void Player::update(float deltaTime) {
    updateAnimation(deltaTime);

    if (getHealth() <= 0) {
        std::chrono::steady_clock::time_point tpEnd = std::chrono::steady_clock::now();
        std::chrono::duration<float> elapsedTime = tpEnd - tpStart;
        float seconds = elapsedTime.count();
        std::cout << "Game over!" << std::endl;
        std::cout << "Time survieved : " << seconds << std::endl;
        std::cout << "Zombies killed : " << score << std::endl;
        exit(0);
    }
}

void Player::keyUpdate(const Uint8* keystates, float deltaTime) {
    float zoomFactor = getZoomFactor();
    float delta = getSpeed() * deltaTime * zoomFactor;
    bool isMoving = false;
    int newDirection = 0;
    float dx = 0, dy = 0;

    if (keystates[SDL_SCANCODE_W]) {
        newDirection = 1;
        dy = -delta;
        isMoving = true;
    }
    if (keystates[SDL_SCANCODE_S]) {
        newDirection = 3;
        dy = delta;
        isMoving = true;
    }
    if (keystates[SDL_SCANCODE_A]) {
        newDirection = 4;
        dx = -delta;
        isMoving = true;
    }
    if (keystates[SDL_SCANCODE_D]) {
        newDirection = 2;
        dx = delta;
        isMoving = true;
    }
    bool needSwitchTexture = false;
    if (isMoving) {
        needSwitchTexture = !getIsWalking(); //Now we move, so we start from the first frame
        move(dx, dy);
        setWalkingDirection(newDirection);
        setIsWalking(true);
    } else {
        needSwitchTexture = getIsWalking(); //Now we stop, so we start from the first frame
        setIsWalking(false);
    }
    if (needSwitchTexture) {
        switchTexture();
    }
}

Zombie* Player::getClosestZombie() {
    Zombie* closestZombie = nullptr;
    float closestDistance = std::numeric_limits<float>::max();
    for (Zombie* Zombie : getMap()->getZombies()) {
        auto [playerX, playerY] = getCoordinates();
        auto [ZombieX, ZombieY] = Zombie->getCoordinates();
        float dx = ZombieX - playerX;
        float dy = ZombieY - playerY;
        float distance = std::sqrt(dx * dx + dy * dy);
        if (distance < closestDistance && distance < 50) {
            closestZombie = Zombie;
            closestDistance = distance;
        }
    }
    return closestZombie;
}

void Player::addScore(int points) {
    score += points;
}

void Player::loadAnimations() {
    SDL_Renderer* renderer = getRenderer();
    SDL_Texture* humanTexture = loadTexture("assets/Characters/Human.png", renderer);
    idleTextures.clear();
    walkTextures.clear();
    
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
        SDL_RenderCopy(renderer, humanTexture, &srcRect, &dstRect);
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
        SDL_RenderCopy(renderer, humanTexture, &srcRect, &dstRect);
        SDL_SetRenderTarget(renderer, nullptr);
        walkTextures.push_back(texture);
    }

    currentTextures = &idleTextures;
    SDL_DestroyTexture(humanTexture);
}

void Player::switchTexture() {
    if (getIsWalking()) {
        currentTextures = &walkTextures;
    } else {
        currentTextures = &idleTextures;
    }
    setCurrentFrame(0);
    setNumFrames(currentTextures->size());
}


void Player::draw(int cameraX, int cameraY) {
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
    SDL_Rect destRect = { drawX - zoomedFrameWidth/2, drawY - zoomedFrameHeight/2, zoomedFrameWidth, zoomedFrameHeight };
    SDL_RenderCopyEx(renderer, texture, nullptr, &destRect, 0, NULL, textureFlip);

    gun->draw(cameraX, cameraY);
}

SDL_RendererFlip Player::getTextureFlip() {
    return textureFlip;
}