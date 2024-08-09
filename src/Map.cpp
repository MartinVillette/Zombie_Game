#include "Map.h"
#include "Zombie.h"
#include <algorithm>
#include <random>
#include "Window.h"


Map::Map(Window* window) : window(window), tileSize(16), mapWidth(1000), mapHeight(1000), loadedRange(1000) {

    windowWidthPtr = window -> getWidthPtr();
    windowHeightPtr = window -> getHeightPtr();
    renderer = window->getRenderer();
    
    //grassTexture = loadTexture("assets/Tilesets/Grass.png", renderer);
    //SDL_Texture* woodenHouseWallsTexture = loadTexture("assets/Tilesets/Wooden_House_Walls_Tilset.png", renderer);


    tiles.resize(mapHeight, std::vector<Tile>(mapWidth));

    for (int y = 0; y < mapHeight; ++y) {
        for (int x = 0; x < mapWidth; ++x) {
            tiles[y][x] = Tile(x, y, "", NULL, renderer, true, tileSize, windowWidthPtr, windowHeightPtr);
        }
    }
}


Map::~Map() {
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
}

Tile* Map::getTilePtr(int x, int y) {
    if (x >= 0 && x < mapWidth && y >= 0 && y < mapHeight) {
        return &tiles[y][x];
    }
    return nullptr;
}

Tile* Map::getTileAtPixelPtr(int pixelX, int pixelY) {
    int tileX = pixelX / tileSize;
    int tileY = pixelY / tileSize;
    return getTilePtr(tileX, tileY);
}

void Map::draw(int cameraX, int cameraY) {
    int min_X = cameraX - *windowWidthPtr / 2;
    int min_Y = cameraY - *windowHeightPtr / 2;

    int nbTilesX = *windowWidthPtr / tileSize + 5;
    int nbTilesY = *windowHeightPtr / tileSize + 5;
    for (int j = 0; j <= nbTilesY; j++) {
        for (int i = 0; i <= nbTilesX; i++) {
            int x = min_X + i * tileSize;
            int y = min_Y + j * tileSize;
            Tile* tilePtr = getTileAtPixelPtr(x, y);
            if (tilePtr) {
                tilePtr->draw(cameraX, cameraY);
            }
        }
    }
}

int Map::getMapWidth() const {return mapWidth;}
int Map::getMapHeight() const {return mapHeight;}
int Map::getTileSize() const {return tileSize;}

std::pair<int, int> Map::worldToTileCoordinates(float x, float y) {
    return {static_cast<int>(x / tileSize), static_cast<int>(y / tileSize)};
}

std::pair<float, float> Map::tileToWorldCoordinates(int tileX, int tileY) {
    return {(tileX + 0.5f) * tileSize, (tileY + 0.5f) * tileSize};
}

void Map::addZombie(Zombie* zombie) {
    zombies.push_back(zombie);
}

void Map::removeZombie(Zombie* zombie) {
    auto it = std::find(zombies.begin(), zombies.end(), zombie);
    if (it != zombies.end()) {
        zombies.erase(it);
    }
}

std::vector<Zombie*> Map::getZombies() {
    return zombies;
}

SDL_Texture* Map::loadTexture(const char* filePath, SDL_Renderer* renderer) {
    SDL_Texture* texture = IMG_LoadTexture(renderer, filePath);
    if (!texture) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load texture: %s", IMG_GetError());
        throw std::runtime_error("Failed to load texture");
    }
    return texture;
}

void Map::updateZombies(float deltaTime){
    Player* playerPtr = window->getPlayerPtr();
    int playerX, playerY;
    std::tie(playerX, playerY) = playerPtr->getCoordinates();

    for (Zombie* zombie : zombies) {
        int zombieX, zombieY;
        std::tie(zombieX, zombieY) = zombie->getCoordinates();
        int distance = std::sqrt(std::pow(playerX - zombieX, 2) + std::pow(playerY - zombieY, 2));

        if (distance < loadedRange) {
            zombie->update(deltaTime);
        }
    }
    
    removeDeadZombies();

    // Random spawn new zombies
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> dis(0.0f, 1.0f);
    if (dis(gen) < 0.01f) {
        float min_X = std::max(0, playerX - *windowWidthPtr);
        float max_X = std::min(playerX + *windowWidthPtr, mapWidth * tileSize);
        float min_Y = std::max(0, playerY - *windowHeightPtr);
        float max_Y = std::min(playerY + *windowHeightPtr, mapHeight * tileSize);

        do {
            float x = (dis(gen)-0.5f) * (max_X - min_X) + playerX;
            float y = (dis(gen)-0.5f) * (max_Y - min_Y) + playerY;
            Tile* tilePtr = getTileAtPixelPtr(x, y);
            if (tilePtr && tilePtr->getIsWalkable()) {
                Zombie* zombie = new Zombie(x, y, window, this);
                zombie->setTarget(player);
                addZombie(zombie);
                return;
            }
        } while (true);
    }
}

void Map::setPlayer(Player* player) {
    this->player = player;
}

void Map::removeDeadZombies() {
    for (auto it = zombies.begin(); it != zombies.end();) {
        if ((*it)->isDead()) {
            it = zombies.erase(it);
            player->addScore(1);
        } else {
            ++it;
        }
    }
}