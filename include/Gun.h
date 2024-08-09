#ifndef GUN_H
#define GUN_H

#include "Bullet.h"
#include <vector>

class Gun {
private:
    int damage;
    int range;
    float fireRate;
    int magazineCapacity;
    int bulletsInMagazine;
    float timeSinceLastShot;
    std::vector<Bullet*> bullets;
    const float bulletSpeed = 300.0f;

public:
    Gun(int damage, int range, float fireRate, int magazineCapacity);
    void shoot(int originX, int originY, float directionX, float directionY);
    void update(float deltaTime, std::vector<Zombie*> zombies);
    void drawBullets(SDL_Renderer* renderer, int cameraX, int cameraY, int windowWidth, int windowHeight);
};

#endif
