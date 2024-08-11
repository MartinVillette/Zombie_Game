#ifndef TILE_H
#define TILE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <unordered_map>

class Window;

class Tile {
private:
    int x, y;
    std::string tileType;
    std::string decoration;
    SDL_Texture* texture;
    SDL_Renderer* renderer;
    bool isWalkable;
    int tileSize;
    std::unordered_map<std::string, SDL_Rect> decorationMap;
    std::unordered_map<std::string, SDL_Rect> tileMap;
    int* windowWidthPtr;
    int* windowHeightPtr;
    
public:
    Tile();
    Tile(int x, int y, std::string tileType, std::string decoration, SDL_Texture* texture, SDL_Renderer* renderer, bool isWalkable, int tileSize, int* windowWidthPtr, int* windowHeightPtr);
    void draw(int cameraX, int cameraY, float zoomFactor);
    bool getIsWalkable() const;
    std::pair<std::string, SDL_Rect> getRandomGrassTile();
    void initializeTileMaps();
};

#endif