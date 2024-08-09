#include "Gun.h"
#include <cmath>
#include "Zombie.h"


Gun::Gun(int damage, int range, float fireRate, int magazineCapacity)
    : damage(damage), range(range), fireRate(fireRate), magazineCapacity(magazineCapacity),
      bulletsInMagazine(magazineCapacity), timeSinceLastShot(0) {}

void Gun::shoot(int originX, int originY, float directionX, float directionY) {
    if (bulletsInMagazine > 0 && timeSinceLastShot >= 1.0f / fireRate) {
        // Normalize direction vector
        float length = std::sqrt(directionX * directionX + directionY * directionY);
        directionX /= length;
        directionY /= length;

        bullets.push_back(new Bullet(originX, originY, directionX, directionY, bulletSpeed, damage, range));
        // bulletsInMagazine--;
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

void Gun::drawBullets(SDL_Renderer* renderer, int cameraX, int cameraY, int windowWidth, int windowHeight) {
    for (Bullet* bullet : bullets) {
        bullet->draw(renderer, cameraX, cameraY, windowWidth, windowHeight);
    }
}