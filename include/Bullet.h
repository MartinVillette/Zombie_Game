// Bullet.h
#pragma once
#include <SDL2/SDL.h>
#include <vector>

class Zombie;

class Bullet {
public:
    Bullet(int originX, int originY, float directionX, float directionY, float speed, int damage, int range);
    bool update(float deltaTime, std::vector<Zombie*> zombies);
    void draw(SDL_Renderer* renderer, int cameraX, int cameraY, int windowWidth, int windowHeight);
    bool isOutOfRange(float range) const;

    float getX() const { return x; }
    float getY() const { return y; }
    int getDamage() const { return damage; }

private:
    int originX, originY;
    float x, y;
    float directionX, directionY;
    float speed;
    int damage;
    int range;
};