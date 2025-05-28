// Bullet.h
#pragma once
#include <SDL2/SDL.h>
#include <vector>

class Zombie;
class Window;

class Bullet {
public:
    Bullet(Window* window, float originX, float originY, float directionX, float directionY, float speed, int damage, int range);
    bool update(float deltaTime, std::vector<Zombie*> zombies);
    // void draw(SDL_Renderer* renderer, int cameraX, int cameraY, int windowWidth, int windowHeight);
    void draw(int cameraX, int cameraY);
    bool isOutOfRange(float range) const;

    float getX() const { return x; }
    float getY() const { return y; }
    int getDamage() const { return damage; }

private:
    Window* window;
    float originX, originY;
    float x, y;
    float directionX, directionY;
    float speed;
    int damage;
    int range;
    int* windowWidthPtr;
    int* windowHeightPtr;
    SDL_Renderer* renderer;
    float* zoomFactorPtr;
};