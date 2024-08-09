#include "Zombie.h"
#include <random>
#include <iostream>

Zombie::Zombie(float x, float y, Window* window, Map* map)
    : Human(x, y, window, map), damage(10), timeSinceLastAttack(0), viewRange(400) {
    setSpeed(50);
}

void Zombie::moveTo(float destinationX, float destinationY) {
    float x = *getX();
    float y = *getY();
    currentPath = getPathfinder()->findPath(x, y, destinationX, destinationY);
}

void Zombie::clearPath() {
    currentPath.clear();
}

bool Zombie::isPathEmpty() {
    return currentPath.empty();
}

void Zombie::update(float deltaTime) {
    if (target) {
        timeSinceLastAttack += deltaTime;
        float targetX = *target->getX();
        float targetY = *target->getY();

        float distance = std::sqrt((targetX - *getX()) * (targetX - *getX()) + (targetY - *getY()) * (targetY - *getY()));
        if (distance > viewRange) {
            return;
        }
        
        // Recalculate path every second or when the path is empty
        static float timeSinceLastPathfinding = 0;
        timeSinceLastPathfinding += deltaTime;
        if (timeSinceLastPathfinding >= 0.5f || isPathEmpty()) {
            clearPath();
            moveTo(targetX, targetY);
            timeSinceLastPathfinding = 0;
        }

        float& x = *getX();
        float& y = *getY();

        if (!isPathEmpty()) {
            
            float nextX = currentPath.front().first;
            float nextY = currentPath.front().second;
            
            float speed = getSpeed();
            
            float dx = nextX - x;
            float dy = nextY - y;
            float distance = std::sqrt(dx*dx + dy*dy);
            
            if (distance < speed * deltaTime) {
                // Reached the next point
                x = nextX;
                y = nextY;
                currentPath.erase(currentPath.begin());
            } else {
                // Move towards the next point
                float ratio = (speed * deltaTime) / distance;
                x += dx * ratio;
                y += dy * ratio;
            }
            
            setIsWalking(true);
            if (std::abs(dx) > std::abs(dy)) {
                setWalkingDirection((dx > 0) ? 2 : 4);  // Right : Left
            } else {
                setWalkingDirection((dy > 0) ? 3 : 1);  // Down : Up
            }

            
        } else {
            setIsWalking(false);
        }
        float distanceToTarget = std::sqrt((targetX - x) * (targetX - x) + (targetY - y) * (targetY - y));
        if (distanceToTarget < 10 && timeSinceLastAttack >= 1) {
            target->takeDamage(damage);
            timeSinceLastAttack = 0;
        }
    }
    updateAnimation(deltaTime);
}

void Zombie::setTarget(Player* target) {
    this->target = target;
}

bool Zombie::isDead() {
    return getHealth() <= 0;
}