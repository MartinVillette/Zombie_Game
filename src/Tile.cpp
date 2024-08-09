#include "Tile.h"
#include "Window.h"

Tile::Tile() : x(0), y(0), tileType("grass1"), texture(nullptr), isWalkable(true), tileSize(16) {
    // Initialize tileMap here if needed
}

Tile::Tile(int x, int y, std::string tileType, SDL_Texture* texture, SDL_Renderer* renderer, bool isWalkable, int tileSize, int* windowWidthPtr, int* windowHeightPtr)
    : x(x), y(y), tileType(tileType), texture(texture), renderer(renderer), isWalkable(isWalkable), tileSize(tileSize), windowWidthPtr(windowWidthPtr), windowHeightPtr(windowHeightPtr) {
        // No need to initialize window here
    }

void Tile::draw(int cameraX, int cameraY) {
    int drawX = (x*tileSize - cameraX) + *windowWidthPtr / 2;
    int drawY = (y*tileSize - cameraY) + *windowHeightPtr / 2;
    SDL_Rect destRect = {drawX, drawY, tileSize, tileSize};
    if (texture){
        SDL_Rect srcRect = tileMap[tileType];
        srcRect.x *= tileSize;
        srcRect.y *= tileSize;
        SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
    } else {
        SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
        SDL_RenderFillRect(renderer, &destRect);
    }
}

bool Tile::getIsWalkable() const { return isWalkable; }