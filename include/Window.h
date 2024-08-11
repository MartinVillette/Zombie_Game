#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <chrono>
#include "Player.h"
#include "Map.h"

class Light;

class Window {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    int width, height;
    Player* player;
    Map* map;
    std::chrono::steady_clock::time_point tp1;
    std::chrono::steady_clock::time_point tp2;
    bool leftMouseDown;
    SDL_Texture* lightMask;
    std::vector<Light*> lights;
    SDL_Texture* backgroundLayer;
    SDL_Texture* lightLayer;
    SDL_Texture* resultLayer;

public:
    Window(int width, int height, const char* title);
    ~Window();
    SDL_Renderer* getRenderer();
    void setPlayer(Player* player);
    void setMap(Map* map);
    void updateScreen();
    bool handleEvents();
    void update(float deltaTime);
    int* getWidthPtr();
    int* getHeightPtr();
    Player* getPlayerPtr();
    void renderLightMask();
    void setLightMask();
    SDL_Texture* getLightMask();
    void addLight(Light* light);
    void removeLight(Light* light);
    Map* getMapPtr();
};

#endif
