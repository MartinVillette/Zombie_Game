#include "Window.h"
#include "Zombie.h"
#include "Light.h"
#include <iostream>
#include <stdexcept>

Window::Window(int width, int height, const char* title)
    : width(width), height(height), player(nullptr), map(nullptr), leftMouseDown(false), lightMask(nullptr) {
    // Initialize SDL2
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not initialize SDL2: %s", SDL_GetError());
        throw std::runtime_error("Failed to initialize SDL2");
    }

    // Initialize SDL2_image
    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not initialize SDL2_image: %s", IMG_GetError());
        SDL_Quit();
        throw std::runtime_error("Failed to initialize SDL2_image");
    }

    // Create a window
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (!window) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not create window: %s", SDL_GetError());
        SDL_Quit();
        throw std::runtime_error("Failed to create window");
    }

    // Create a renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not create renderer: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        throw std::runtime_error("Failed to create renderer");
    }

    // Initialize the lightMask texture
    setLightMask();

    // Initialize time points
    tp1 = std::chrono::steady_clock::now();
    tp2 = std::chrono::steady_clock::now();
}

Window::~Window() {
    // Cleanup
    if (lightMask) {
        SDL_DestroyTexture(lightMask);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

SDL_Renderer* Window::getRenderer() {
    return renderer;
}

void Window::setPlayer(Player* player) {
    this->player = player;
}

void Window::setMap(Map* map) {
    this->map = map;
}

void Window::updateScreen() {
    // Clear screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Set background to black
    SDL_RenderClear(renderer);

    int playerX, playerY;
    std::tie(playerX, playerY) = player->getCoordinates();

    // Draw map
    if (map) {
        map->draw(playerX, playerY);
    }

    // Draw zombies
    for (Zombie* zombie : map->getZombies()) {
        zombie->draw(playerX, playerY);
    }
    
    // Draw player
    if (player) {
        player->draw(playerX, playerY);
    }

    // Draw bullets
    player->getGun()->drawBullets(renderer, playerX, playerY, width, height);

    // Render light mask
    renderLightMask();

    // Present the renderer
    SDL_RenderPresent(renderer);
}

void Window::setLightMask() {
    if (lightMask) {
        SDL_DestroyTexture(lightMask); // Clean up any existing texture
    }
    
    lightMask = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
    if (!lightMask) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not create darkness texture: %s", SDL_GetError());
        throw std::runtime_error("Failed to create darkness texture");
    }

    // Set the blending mode for the darkness texture
    if (SDL_SetTextureBlendMode(lightMask, SDL_BLENDMODE_BLEND) != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not set texture blend mode: %s", SDL_GetError());
        throw std::runtime_error("Failed to set texture blend mode");
    }
}

SDL_Texture* Window::getLightMask() {
    return lightMask;
}
void Window::renderLightMask() {
    if (!lightMask) {
        std::cerr << "Light mask texture is not initialized!" << std::endl;
        return;
    }

    // Set render target to lightMask
    if (SDL_SetRenderTarget(renderer, lightMask) != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not set render target: %s", SDL_GetError());
        return;
    }

    // Clear the lightMask with a dark color
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
    SDL_RenderClear(renderer);

    // Set blend mode to add for rendering lights
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_ADD);

    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255); // how much you can see in the dark
    SDL_RenderFillRect(renderer, NULL);

    // Render all lights
    for (Light* light : lights) {
        light->render();
    }

    // Reset the render target to the default (screen)
    SDL_SetRenderTarget(renderer, NULL);

    // Set the blending mode for the lightMask
    SDL_SetTextureBlendMode(lightMask, SDL_BLENDMODE_MOD);

    // Render the lightMask texture onto the screen
    SDL_RenderCopy(renderer, lightMask, NULL, NULL);
}

void Window::addLight(Light* light) {
    lights.push_back(light);
}

void Window::removeLight(Light* light) {
    for (auto it = lights.begin(); it != lights.end();) {
        if (*it == light) {
            it = lights.erase(it);
        } else {
            ++it;
        }
    }
}

bool Window::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                return false;
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    width = event.window.data1;
                    height = event.window.data2;
                    setLightMask(); // Recreate the lightMask texture with new dimensions
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                leftMouseDown = true;
                break;
            case SDL_MOUSEBUTTONUP:
                leftMouseDown = false;
                break;
            case SDL_KEYDOWN:
                player->keyDown(event.key.keysym.sym);
                break;
        }
    }

    tp2 = std::chrono::steady_clock::now();
    std::chrono::duration<float> elapsedTime = tp2 - tp1;
    tp1 = tp2;
    float deltaTime = elapsedTime.count();

    update(deltaTime);

    return true;
}

void Window::update(float deltaTime) {
    if (leftMouseDown) {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        player->mouseUpdate(mouseX, mouseY);
    }

    const Uint8* keystates = SDL_GetKeyboardState(NULL);
    player->keyUpdate(keystates, deltaTime);

    player->update(deltaTime);

    map->updateZombies(deltaTime);

    player->getGun()->update(deltaTime, map->getZombies());
} 

int* Window::getWidthPtr() { return &width; }
int* Window::getHeightPtr() { return &height; }

Player* Window::getPlayerPtr() {
    return player;
}