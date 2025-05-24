#include "Bullet.h"
#include <cmath>
#include <iostream>
#include "Zombie.h"
#include <vector>
#include "Window.h"

Bullet::Bullet(Window* window, int originX, int originY, float directionX, float directionY, float speed, int damage, int range)
    : window(window), originX(originX), originY(originY), directionX(directionX), directionY(directionY), speed(speed), damage(damage), range(range) {
    x = (float) originX;
    y = (float) originY;

    windowWidthPtr = window->getWidthPtr();
    windowHeightPtr = window->getHeightPtr();
    renderer = window->getRenderer();
    zoomFactorPtr = window->getMapPtr()->getZoomFactorPtr();
}

bool Bullet::update(float deltaTime, std::vector<Zombie*> zombies) {
    float zoomFactor = *zoomFactorPtr;
    x += directionX * speed * deltaTime * zoomFactor;
    y += directionY * speed * deltaTime * zoomFactor;

    // if bullet touches a zombie, then the bullet is destroyed and zombie is damaged
    for (Zombie* zombie : zombies) {
        float dx = x - *zombie->getX();
        float dy = y - *zombie->getY();
        float distance = std::sqrt(dx*dx + dy*dy);
        if (distance < 10) {
            zombie->takeDamage(damage);
            return true;
        }
    }
    return false;
}

void Bullet::draw(int cameraX, int cameraY) {
    // Adjust bullet position relative to the camera
    int windowWidth = *windowWidthPtr;
    int windowHeight = *windowHeightPtr;
    float zoomFactor = *zoomFactorPtr;
    int bulletSize = 4 * zoomFactor;

    int drawX = static_cast<int>(x - cameraX + windowWidth / 2.0f - bulletSize/2);
    int drawY = static_cast<int>(y - cameraY + windowHeight / 2.0f - bulletSize/2);
    
    // Draw bullet
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Yellow color for bullets
    SDL_Rect bulletRect = {drawX, drawY, bulletSize, bulletSize}; // Slight offset to center the bullet
    SDL_RenderFillRect(renderer, &bulletRect);
}


bool Bullet::isOutOfRange(float range) const {
    float dx = x - originX;
    float dy = y - originY;
    float distance = std::sqrt(dx*dx + dy*dy) / *zoomFactorPtr;
    return distance > range;
}