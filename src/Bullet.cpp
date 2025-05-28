#include "Bullet.h"
#include <cmath>
#include <iostream>
#include "Zombie.h"
#include <vector>
#include "Window.h"

Bullet::Bullet(Window* window, float originX, float originY, float directionX, float directionY, float speed, int damage, int range)
    : window(window), originX(originX), originY(originY), directionX(directionX), directionY(directionY), speed(speed), damage(damage), range(range) {
    x = originX;
    y = originY;

    windowWidthPtr = window->getWidthPtr();
    windowHeightPtr = window->getHeightPtr();
    renderer = window->getRenderer();
    zoomFactorPtr = window->getMapPtr()->getZoomFactorPtr();
}

bool Bullet::update(float deltaTime, std::vector<Zombie*> zombies) {
    // Move bullet in world coordinates
    x += directionX * speed * deltaTime;
    y += directionY * speed * deltaTime;

    // Check collision with zombies using sprite dimensions
    for (Zombie* zombie : zombies) {
        auto [zombieX, zombieY] = zombie->getCoordinates();
        auto [frameWidth, frameHeight] = zombie->getFrameSize();
        
        // Use half the sprite size for collision detection
        float zombieHalfWidth = frameWidth / 2.0f;
        float zombieHalfHeight = frameHeight / 2.0f;
        
        // Check if bullet is within zombie's bounding box
        if (x >= zombieX - zombieHalfWidth && x <= zombieX + zombieHalfWidth &&
            y >= zombieY - zombieHalfHeight && y <= zombieY + zombieHalfHeight) {
            zombie->takeDamage(damage);
            return true;
        }
    }
    
    // Check range in world coordinates
    float travelDistance = sqrt(pow(x - originX, 2) + pow(y - originY, 2));
    return travelDistance > range;
}

void Bullet::draw(int cameraX, int cameraY) {
    int windowWidth = *windowWidthPtr;
    int windowHeight = *windowHeightPtr;
    float zoomFactor = *zoomFactorPtr;
    
    // Convert world coordinates to screen coordinates with zoom
    int drawX = static_cast<int>((x - cameraX) * zoomFactor + windowWidth / 2.0f);
    int drawY = static_cast<int>((y - cameraY) * zoomFactor + windowHeight / 2.0f);
    
    int bulletSize = static_cast<int>(4 * zoomFactor);
    
    // Center the bullet
    drawX -= bulletSize / 2;
    drawY -= bulletSize / 2;
    
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_Rect bulletRect = {drawX, drawY, bulletSize, bulletSize};
    SDL_RenderFillRect(renderer, &bulletRect);
}


bool Bullet::isOutOfRange(float range) const {
    float dx = x - originX;
    float dy = y - originY;
    float travelDistance = sqrt(pow(dx, 2) + pow(dy, 2));
    return travelDistance > range;
}