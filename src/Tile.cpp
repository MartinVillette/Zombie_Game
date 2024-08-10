#include "Tile.h"
#include "Window.h"
#include <random>

Tile::Tile() : x(0), y(0), decoration("none"), texture(nullptr), isWalkable(true), tileSize(16) {
    // Empty constructor
}

Tile::Tile(int x, int y, std::string decoration, SDL_Texture* texture, SDL_Renderer* renderer, bool isWalkable, int tileSize, int* windowWidthPtr, int* windowHeightPtr)
    : x(x), y(y), decoration(decoration), texture(texture), renderer(renderer), isWalkable(isWalkable), tileSize(tileSize), windowWidthPtr(windowWidthPtr), windowHeightPtr(windowHeightPtr) {

    // Initialize static maps only once
    static bool initialized = false;
    if (!initialized) {
        initialized = true;
        initializeTileMaps();
    }
}

void Tile::initializeTileMaps() {
    decorationMap = {
        {"bush", {30, 1, 16, 16}},
        // Add more tile types here
    };

    grassTileMap = {
        {"grass1", {1, 1, 16, 16}},
        {"grass2", {1, 2, 16, 16}},
        {"grass3", {1, 3, 16, 16}},
    };
}

std::pair<std::string, SDL_Rect> Tile::getRandomGrassTile() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> distrib(0, grassTileMap.size() - 1);

    auto it = grassTileMap.begin();
    std::advance(it, distrib(gen));
    return {it->first, it->second};
}

void Tile::draw(int cameraX, int cameraY) {
    int drawX = (x * tileSize - cameraX) + *windowWidthPtr / 2;
    int drawY = (y * tileSize - cameraY) + *windowHeightPtr / 2;
    SDL_Rect destRect = {drawX, drawY, tileSize, tileSize};
    
    if (texture) {
        std::pair<std::string, SDL_Rect> randomGrassTile = getRandomGrassTile();
        SDL_Rect srcRect = randomGrassTile.second;
        srcRect.x *= tileSize;
        srcRect.y *= tileSize;

        SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
        if (!decoration.empty() && decoration != "none") {
            auto it = decorationMap.find(decoration);
            if (it != decorationMap.end()) {
                SDL_Rect decorationSrcRect = it->second;
                decorationSrcRect.x *= tileSize;
                decorationSrcRect.y *= tileSize;
                SDL_RenderCopy(renderer, texture, &decorationSrcRect, &destRect);
            }
        }
    } else {
        SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
        SDL_RenderFillRect(renderer, &destRect);
    }
}

bool Tile::getIsWalkable() const {
    return isWalkable;
}
