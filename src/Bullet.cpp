#include "Bullet.h"
#include <cmath>
#include <iostream>
#include "Zombie.h"
#include <vector>

Bullet::Bullet(int originX, int originY, float directionX, float directionY, float speed, int damage, int range)
    : originX(originX), originY(originY), directionX(directionX), directionY(directionY), speed(speed), damage(damage), range(range) {
    x = (float) originX;
    y = (float) originY;
}

bool Bullet::update(float deltaTime, std::vector<Zombie*> zombies) {
    x += directionX * speed * deltaTime;
    y += directionY * speed * deltaTime;

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

void Bullet::draw(SDL_Renderer* renderer, int cameraX, int cameraY, int windowWidth, int windowHeight) {
    // Adjust bullet position relative to the camera
    int drawX = static_cast<int>(x - cameraX + windowWidth / 2.0f);
    int drawY = static_cast<int>(y - cameraY + windowHeight / 2.0f);
    
    // Draw bullet
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Yellow color for bullets
    SDL_Rect bulletRect = {drawX - 2, drawY - 2, 4, 4}; // Slight offset to center the bullet
    SDL_RenderFillRect(renderer, &bulletRect);
}

bool Bullet::isOutOfRange(float range) const {
    float dx = x - originX;
    float dy = y - originY;
    return std::sqrt(dx*dx + dy*dy) > range;
}