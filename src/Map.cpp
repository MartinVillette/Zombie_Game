#include "Map.h"
#include "Zombie.h"
#include <algorithm>
#include <random>
#include "Window.h"


Map::Map(Window* window) : window(window), tileSize(16), mapWidth(50), mapHeight(50), loadedRange(1000), zoomFactor(1.0f) {

    windowWidthPtr = window->getWidthPtr();
    windowHeightPtr = window->getHeightPtr();
    renderer = window->getRenderer();
    
    // Load texture once for the entire map
    texture = loadTexture("assets/tileset.png");

    // Precompute decorations for all tiles
    mapData.resize(mapHeight, std::vector<TileType>(mapWidth));
    decorationData.resize(mapHeight, std::vector<std::string>(mapWidth, "none"));

    generateTerrain();
    generateDecorations();
    createTiles();
}   

Map::~Map() {
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
}

void Map::generateTerrain() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(0, 100);

    for (int y = 0; y < mapHeight; ++y) {
        for (int x = 0; x < mapWidth; ++x) {
            int randomValue = distr(gen);
            if (randomValue < 60) {
                mapData[y][x] = TileType::Grass1;
            } else if (randomValue < 80) {
                mapData[y][x] = TileType::Grass2;
            } else {
                mapData[y][x] = TileType::Grass3;
            }
        }
    }
}

void Map::createTiles() {
    tiles.resize(mapHeight, std::vector<Tile>(mapWidth));

    for (int y = 0; y < mapHeight; ++y) {
        for (int x = 0; x < mapWidth; ++x) {
            std::string tileType;
            bool isWalkable = true;

            switch (mapData[y][x]) {
                case TileType::Grass1:
                    tileType = "grass1";
                    break;
                case TileType::Grass2:
                    tileType = "grass2";
                    break;
                case TileType::Grass3:
                    tileType = "grass3";
                    break;
            }

            if (decorationData[y][x] == "tree" || decorationData[y][x] == "bush") {
                isWalkable = false;
            }
            
            tiles[y][x] = Tile(x, y, tileType, decorationData[y][x], texture, renderer, isWalkable, tileSize, windowWidthPtr, windowHeightPtr);
        }
    }
}

std::string Map::getDecoration() {
    static std::unordered_map<std::string, int> mapDecorationsChances = {
        {"bush", 10},
        // Add more decorations here
    };

    static int totalChances = std::accumulate(mapDecorationsChances.begin(), mapDecorationsChances.end(), 0,
        [](int sum, const auto& pair) { return sum + pair.second; });

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(1, totalChances);
    int randomNum = distrib(gen);

    int cumulativeChance = 0;
    for (const auto& pair : mapDecorationsChances) {
        cumulativeChance += pair.second;
        if (randomNum <= cumulativeChance) {
            return pair.first;
        }
    }

    return "none"; // Default if no decoration is selected
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
    int zoomedTileSize = static_cast<int>(tileSize * zoomFactor);
    int min_X = cameraX - (*windowWidthPtr / 2) / zoomFactor;
    int min_Y = cameraY - (*windowHeightPtr / 2) / zoomFactor;

    int nbTilesX = (*windowWidthPtr / zoomedTileSize) + 2;
    int nbTilesY = (*windowHeightPtr / zoomedTileSize) + 2;

    for (int j = 0; j <= nbTilesY; j++) {
        for (int i = 0; i <= nbTilesX; i++) {
            int x = min_X + i * tileSize;
            int y = min_Y + j * tileSize;
            Tile* tilePtr = getTileAtPixelPtr(x, y);
            if (tilePtr) {
                tilePtr->draw(cameraX, cameraY, zoomFactor);
            }
        }
    }
}

int Map::getMapWidth() const { return mapWidth; }
int Map::getMapHeight() const { return mapHeight; }
int Map::getTileSize() const { return tileSize; }

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

SDL_Texture* Map::loadTexture(const char* filePath) {
    SDL_Texture* text = IMG_LoadTexture(renderer, filePath);
    if (!text) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load texture: %s", IMG_GetError());
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL Error: %s", SDL_GetError());
        throw std::runtime_error("Failed to load texture");
    }
    return text;
}

void Map::updateZombies(float deltaTime) {
    Player* playerPtr = window->getPlayerPtr();
    int playerX, playerY;
    std::tie(playerX, playerY) = playerPtr->getCoordinates();

    for (Zombie* zombie : zombies) {
        int zombieX, zombieY;
        std::tie(zombieX, zombieY) = zombie->getCoordinates();
        int dx = playerX - zombieX;
        int dy = playerY - zombieY;
        int distance = std::sqrt(dx * dx + dy * dy);

        if (distance < loadedRange) {
            zombie->update(deltaTime);
        }
    }
    removeDeadZombies();
    spawnZombie();
}

void Map::spawnZombie(){
    Player* playerPtr = window->getPlayerPtr();
    int playerX, playerY;
    std::tie(playerX, playerY) = playerPtr->getCoordinates();

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
            float x = (dis(gen) - 0.5f) * (max_X - min_X) + playerX;
            float y = (dis(gen) - 0.5f) * (max_Y - min_Y) + playerY;
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

Player Map::spawnPlayer() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribX(0,mapWidth);
    std::uniform_int_distribution<int> distribY(0,mapHeight);

    do {
        int x = distribX(gen);
        int y = distribY(gen);

        Tile* tilePtr = getTilePtr(x, y);
        if (tilePtr && tilePtr->getIsWalkable()) {
            Player player = Player(x * tileSize, y*tileSize, window, this);
            setPlayer(&player);
            return player;
        }
    } while (true);
}

void Map::setZoomFactor(float factor) {
    zoomFactor = std::max(0.1f, std::min(factor, 5.0f)); // Limit zoom between 0.1x and 5x
}

float Map::getZoomFactor() const {
    return zoomFactor;
}

float* Map::getZoomFactorPtr() {
    return &zoomFactor;
}