#ifndef GUN_H
#define GUN_H

#include "Bullet.h"
#include <vector>

class Window;
class Player;

class Gun {
private:
    Window* window;
    Player* playerPtr;
    int damage;
    int range;
    float fireRate;
    int magazineCapacity;
    int bulletsInMagazine;
    float timeSinceLastShot;
    std::vector<Bullet*> bullets;
    const float bulletSpeed = 300.0f;
    SDL_Texture* gunTexture;
    int rotatedEndX, rotatedEndY;

public:
    Gun(Window* window, Player* playerPtr, int damage, int range, float fireRate, int magazineCapacity);
    void shoot(float directionX, float directionY);
    void update(float deltaTime, std::vector<Zombie*> zombies);
    void drawBullets(int cameraX, int cameraY);
    SDL_Texture* loadTexture(const char* filePath, SDL_Renderer* renderer);
    void draw(int cameraX, int cameraY);
};

#endif
