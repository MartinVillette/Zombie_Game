#ifndef HUMAN_H
#define HUMAN_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <stdexcept>
#include "Pathfinder.h"
#include <iostream>

class Map;
class Window;

class Human {
private:
    float x, y;
    float dx, dy;
    Window* window;
    Map* map;   
    SDL_Renderer* renderer;
    int frameWidth, frameHeight;
    int currentFrame;
    float frameTime;
    float timeSinceLastFrame;
    int numFrames;
    float speed;
    bool isWalking;
    int walkingDirection;
    // SDL_Texture* zombieTexture;
    // SDL_Texture* humanTexture;

    // SDL_RendererFlip textureFlip;
    bool isPlayer;
    Pathfinder* pathfinder;
    int health;
    int* windowWidthPtr;
    int* windowHeightPtr;
    float* zoomFactorPtr;

public:
    Human(float x, float y, Window* window, Map* map);
    virtual ~Human();
    void move(float dx, float dy);
    float getSpeed() const;
    // void draw(int cameraX, int cameraY);
    bool getIsWalking();
    int getWalkingDirection();
    int getCurrentFrame();
    void setIsWalking(bool isWalking);
    void setWalkingDirection(int walkingDirection);
    void setCurrentFrame(int currentFrame);
    std::pair<float, float> getCoordinates() const;
    Map* getMap();
    void updateHuman(float deltaTime);
    void updateAnimation(float deltaTime);
    float* getX();
    float* getY();
    void setIsPlayer(bool isPlayer);
    void setSpeed(float newSpeed);
    Pathfinder* getPathfinder();
    void takeDamage(int damage);
    int getHealth();
    void setHealth(int newHealth);
    std::pair<int, int> getWindowSize();
    float getZoomFactor();
    SDL_Texture* loadTexture(const char* filePath, SDL_Renderer* renderer);
    SDL_Renderer* getRenderer();
    std::pair<int, int> getFrameSize();
    void setNumFrames(int numFrames);
};

#endif
