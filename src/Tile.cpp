#include "Tile.h"
#include "Window.h"
#include <random>

Tile::Tile() : x(0), y(0), tileType("grass1"), decoration("none"), texture(nullptr), isWalkable(true), tileSize(16) {
    // Empty constructor
}

Tile::Tile(int x, int y, std::string tileType, std::string decoration, SDL_Texture* texture, SDL_Renderer* renderer, bool isWalkable, int tileSize, int* windowWidthPtr, int* windowHeightPtr)
    : x(x), y(y), tileType(tileType), decoration(decoration), texture(texture), renderer(renderer), isWalkable(isWalkable), tileSize(tileSize), windowWidthPtr(windowWidthPtr), windowHeightPtr(windowHeightPtr) {
    
    initializeTileMaps();
}

void Tile::initializeTileMaps() {
    decorationMap["bush"] = {30, 1, 16, 16};
    decorationMap["tree"] = {52, 3, 16, 48};

    tileMap["grass1"] = {2, 1, 16, 16};
    tileMap["grass2"] = {1, 2, 16, 16};
    tileMap["grass3"] = {1, 3, 16, 16};
}


void Tile::draw(int cameraX, int cameraY, float zoomFactor) {
    int zoomedTileSize = static_cast<int>(tileSize * zoomFactor);
    int drawX = static_cast<int>((x * tileSize - cameraX) * zoomFactor + *windowWidthPtr / 2);
    int drawY = static_cast<int>((y * tileSize - cameraY) * zoomFactor + *windowHeightPtr / 2);
    SDL_Rect destRect = {drawX, drawY, zoomedTileSize, zoomedTileSize};

    if (texture) {
        SDL_Rect srcRect = tileMap[tileType];
        srcRect.x *= tileSize;
        srcRect.y *= tileSize;

        SDL_RenderCopy(renderer, texture, &srcRect, &destRect);

        if (!decoration.empty() && decoration != "none") {
            auto it = decorationMap.find(decoration);
            if (it != decorationMap.end()) {
                SDL_Rect decorationSrcRect = it->second;
                SDL_Rect decorationDestRect = destRect;
                decorationDestRect.h = static_cast<int>(decorationSrcRect.h * zoomFactor);
                decorationDestRect.y -= (decorationDestRect.h - zoomedTileSize);
                decorationSrcRect.x *= tileSize;
                decorationSrcRect.y *= tileSize;
                SDL_RenderCopy(renderer, texture, &decorationSrcRect, &decorationDestRect);
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
