#include "Player.h"
#include "Zombie.h"
#include "Light.h"
#include "Window.h"
#include <iostream>

Player::Player(float x, float y, Window* window, Map* map)
    : Human(x, y, window, map), score(0) {
    setIsPlayer(true);
    gun = new Gun(20, 200, 20, 10);
    tpStart = std::chrono::steady_clock::now();
    light = new Light(&x, &y, 100, 10, 200, window);
    window->addLight(light);
}

void Player::keyDown(SDL_Keycode key) {
    if (key == SDLK_SPACE) {
        //talk();
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

    gun->shoot(playerX, playerY, directionX, directionY);
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
    float delta = getSpeed() * deltaTime;
    bool isMoving = false;
    int newDirection = 0;
    float dx = 0, dy = 0;

    if (keystates[SDL_SCANCODE_Z]) {
        newDirection = 1;
        dy = -delta;
        isMoving = true;
    }
    if (keystates[SDL_SCANCODE_S]) {
        newDirection = 3;
        dy = delta;
        isMoving = true;
    }
    if (keystates[SDL_SCANCODE_Q]) {
        newDirection = 4;
        dx = -delta;
        isMoving = true;
    }
    if (keystates[SDL_SCANCODE_D]) {
        newDirection = 2;
        dx = delta;
        isMoving = true;
    }

    if (isMoving) {
        move(dx, dy);
        setWalkingDirection(newDirection);
        setIsWalking(true);
    } else {
        setIsWalking(false);
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