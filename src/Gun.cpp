#include "Gun.h"
#include <cmath>
#include "Zombie.h"
#include "Window.h"
#include "Player.h"


Gun::Gun(Window* window, Player* playerPtr, int damage, int range, float fireRate, int magazineCapacity)
    : window(window), playerPtr(playerPtr), damage(damage), range(range), fireRate(fireRate), magazineCapacity(magazineCapacity),
      bulletsInMagazine(magazineCapacity) {
        timeSinceLastShot = 0;
        gunTexture = loadTexture("assets/Guns/Pistol.png", window->getRenderer());
    }

void Gun::shoot(float directionX, float directionY) {
    if (bulletsInMagazine > 0 && timeSinceLastShot >= 1.0f / fireRate) {
        // Normalize direction vector
        float length = std::sqrt(directionX * directionX + directionY * directionY);
        directionX /= length;
        directionY /= length;

        // Use player's world coordinates as origin (not screen coordinates)
        float playerX, playerY;
        std::tie(playerX, playerY) = playerPtr->getCoordinates();
        
        bullets.push_back(new Bullet(window, playerX, playerY, directionX, directionY, bulletSpeed, damage, range));
        timeSinceLastShot = 0;
    }
}

void Gun::update(float deltaTime, std::vector<Zombie*> zombies) {
    timeSinceLastShot += deltaTime;
    auto it = bullets.begin();
    while (it != bullets.end()) {
        bool hasTouched = (*it)->update(deltaTime, zombies);
        if ((*it)->isOutOfRange(range) || hasTouched) {
            delete *it;
            it = bullets.erase(it);
        } else {
            ++it;
        }
    }
}

void Gun::drawBullets(int cameraX, int cameraY) {
    for (Bullet* bullet : bullets) {
        bullet->draw(cameraX, cameraY);
    }
}


SDL_Texture* Gun::loadTexture(const char* filePath, SDL_Renderer* renderer) {
    SDL_Texture* texture = IMG_LoadTexture(renderer, filePath);
    if (!texture) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load texture: %s", IMG_GetError());
        throw std::runtime_error("Failed to load texture");
    }
    return texture;
}


void Gun::draw(int cameraX, int cameraY) {
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    
    SDL_Rect src = {0, 0, 32, 32};

    float x,y;
    std::tie(x, y) = playerPtr->getCoordinates();
    float zoomFactor = playerPtr->getZoomFactor();

    int windowWidth, windowHeight;
    windowWidth = *window->getWidthPtr();
    windowHeight = *window->getHeightPtr();

    int frameWidth = 12;
    int frameHeight = 12;
    int zoomedFrameWidth = static_cast<int>(frameWidth * zoomFactor);
    int zoomedFrameHeight = static_cast<int>(frameHeight * zoomFactor);
    
    int drawX = static_cast<int>((x - cameraX - 7) * zoomFactor + (windowWidth / 2));
    int drawY = static_cast<int>((y - cameraY - 4) * zoomFactor + (windowHeight / 2));

    double angle = atan2(mouseY - drawX, mouseX - drawY) * 180 / M_PI + 10;

    SDL_Point rotationPoint;
    rotationPoint.x = 7 * zoomedFrameWidth / 32; // center horizontally
    rotationPoint.y = 20 * zoomedFrameHeight / 32;

    SDL_Rect dest = {drawX, drawY, zoomedFrameWidth, zoomedFrameHeight};
    SDL_RenderCopyEx(window->getRenderer(), gunTexture, &src, &dest, angle, &rotationPoint, SDL_FLIP_NONE);
}