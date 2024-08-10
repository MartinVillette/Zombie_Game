#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

#include "Window.h"
#include "Map.h"
#include "Player.h"
#include "Zombie.h"

const int WINDOW_WIDTH = 640, WINDOW_HEIGHT = 480;

int main(int argc, char* argv[]) {
    try {
        Window window(WINDOW_WIDTH, WINDOW_HEIGHT, "RPG Game");
        Map map(&window);

        Player player = map.spawnPlayer(); // Adjust radius for texture size

        window.setPlayer(&player);
        window.setMap(&map);

        // Zombie zombie(1000, 100, &window, &map);
        // zombie.setTarget(&player);
        // map.addZombie(&zombie); 


        while (window.handleEvents()) {
            // Main loop
            window.updateScreen();
        }
    } catch (const std::exception& e) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error: %s", e.what());
        return 1;
    }
    return 0;
}
