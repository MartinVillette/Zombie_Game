#ifndef MAP_H
#define MAP_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <stdexcept>
#include "Tile.h"

class Zombie;
class Player;
class Window;

class Map {
private:

    Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;

    int tileSize;
    int mapWidth, mapHeight;
    int loadedRange;
    int* windowWidthPtr;
    int* windowHeightPtr;
    std::vector<std::vector<Tile>> tiles;
    std::vector<Zombie*> zombies;
    Player* player;
    float zoomFactor;

    SDL_Texture* loadTexture(const char* filePath);

    enum class TileType {
        Grass1,
        Grass2,
        Grass3,
    };
    std::vector<std::vector<TileType>> mapData;
    std::vector<std::vector<std::string>> decorationData;


public:
    Map(Window* window);
    ~Map();
    Tile* getTilePtr(int x, int y);
    Tile* getTileAtPixelPtr(int pixelX, int pixelY);
    void draw(int cameraX, int cameraY);
    int getMapWidth() const;
    int getMapHeight() const;
    int getTileSize() const;
    std::pair<int, int> worldToTileCoordinates(float x, float y);
    std::pair<float, float> tileToWorldCoordinates(int tileX, int tileY);
    void addZombie(Zombie* zombie);
    void removeZombie(Zombie* zombie);
    void updateZombies(float deltaTime);
    void removeDeadZombies();
    std::vector<Zombie*> getZombies();
    void setPlayer(Player* player);
    // void createMap();
    std::string getDecoration();
    Player spawnPlayer();
    void generateTerrain();
    void smoothTerrain(int iterations);
    void createBoundaries();
    void generateDecorations();
    void createTiles();
    void setZoomFactor(float factor);
    float getZoomFactor() const;
    float* getZoomFactorPtr();
    void spawnZombie();
};

#endif
