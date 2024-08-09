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

public:
    Zombie(float x, float y, Window* window, Map* map);
    void moveTo(float destinationX, float destinationY);
    void update(float deltaTime);
    void setTarget(Player* target);
    void clearPath();
    bool isPathEmpty();
    bool isDead();
};

#endif
