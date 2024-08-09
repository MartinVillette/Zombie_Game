#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <limits>
#include "Human.h"
#include "Gun.h"
#include <chrono>

class Zombie;
class Light;

class Player : public Human {
private:
    Gun* gun;
    Light* light;
    int score;
    std::chrono::steady_clock::time_point tpStart;
    
public:
    Player(float x, float y, Window* window, Map* map);
    void keyDown(SDL_Keycode key);
    void update(float deltaTime);
    void keyUpdate(const Uint8* keystates, float deltaTime);
    Zombie* getClosestZombie();
    void mouseUpdate(int mouseX, int mouseY);
    Gun* getGun() { return gun; }
    void addScore(int score);
};

#endif
