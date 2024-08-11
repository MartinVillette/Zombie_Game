#ifndef Zombie_H
#define Zombie_H

#include <string>
#include <random>
#include "Human.h"
#include "Player.h"
#include "Pathfinder.h"

class Zombie : public Human {
private:
    std::vector<std::pair<float, float>> currentPath;
    Player* target;
    int damage;
    float timeSinceLastAttack;
    int viewRange;
    SDL_RendererFlip textureFlip;

    std::vector<SDL_Texture*>* currentTextures;
    std::vector<SDL_Texture*> idleTextures;
    std::vector<SDL_Texture*> walkTextures;
    std::vector<SDL_Texture*> sleepTextures;
    std::vector<SDL_Texture*> attackTextures;

public:
    Zombie(float x, float y, Window* window, Map* map);
    void moveTo(float destinationX, float destinationY);
    void update(float deltaTime);
    void setTarget(Player* target);
    void clearPath();
    bool isPathEmpty();
    bool isDead();
    void draw(int cameraX, int cameraY);
    void loadAnimations();
    void switchTexture();
};

#endif
