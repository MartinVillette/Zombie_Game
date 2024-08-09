#include "Pathfinder.h"
#include <queue>
#include <vector>
#include <cmath>
#include <algorithm>
#include <memory>

Pathfinder::Pathfinder(Map* map) : map(map) {}

std::vector<std::pair<float, float>> Pathfinder::findPath(float startX, float startY, float endX, float endY) {
    int startTileX = startX / map->getTileSize();
    int startTileY = startY / map->getTileSize();
    int endTileX = endX / map->getTileSize();
    int endTileY = endY / map->getTileSize();

    std::priority_queue<Node*, std::vector<Node*>, decltype(&compareNodes)> openList(compareNodes);
    std::vector<std::vector<Node*>> nodeMap(map->getMapHeight(), std::vector<Node*>(map->getMapWidth(), nullptr));

    Node* startNode = new Node(startTileX, startTileY);
    startNode->g = 0;
    startNode->h = heuristic(startTileX, startTileY, endTileX, endTileY);
    startNode->f = startNode->g + startNode->h;

    openList.push(startNode);
    nodeMap[startTileY][startTileX] = startNode;

    while (!openList.empty()) {
        Node* current = openList.top();
        openList.pop();

        if (current->x == endTileX && current->y == endTileY) {
            // Path found, reconstruct and return it
            std::vector<std::pair<float, float>> path;
            while (current != nullptr) {
                path.push_back({current->x * map->getTileSize() + map->getTileSize() / 2.0f,
                                current->y * map->getTileSize() + map->getTileSize() / 2.0f});
                current = current->parent;
            }
            std::reverse(path.begin(), path.end());

            // Clean up
            for (auto& row : nodeMap) {
                for (auto node : row) {
                    delete node;
                }
            }
            path.erase(path.begin());
            return path;
        }

        for (int dy = -1; dy <= 1; dy++) {
            for (int dx = -1; dx <= 1; dx++) {
                if (dx == 0 && dy == 0) continue;

                int newX = current->x + dx;
                int newY = current->y + dy;

                if (newX < 0 || newX >= map->getMapWidth() || newY < 0 || newY >= map->getMapHeight()) {
                    continue;
                }

                Tile* tilePtr = map->getTilePtr(newX, newY);
                if (!tilePtr->getIsWalkable()) {
                    continue;
                }

                float newG = current->g + ((dx == 0 || dy == 0) ? 1.0f : 1.414f);

                Node* neighbor = nodeMap[newY][newX];
                if (neighbor == nullptr) {
                    neighbor = new Node(newX, newY, current);
                    neighbor->g = newG;
                    neighbor->h = heuristic(newX, newY, endTileX, endTileY);
                    neighbor->f = neighbor->g + neighbor->h;
                    openList.push(neighbor);
                    nodeMap[newY][newX] = neighbor;
                } else if (newG < neighbor->g) {
                    neighbor->parent = current;
                    neighbor->g = newG;
                    neighbor->f = neighbor->g + neighbor->h;
                    // Re-add to open list to update position
                    openList.push(neighbor);
                }
            }
        }
    }

    // No path found, clean up
    for (auto& row : nodeMap) {
        for (auto node : row) {
            delete node;
        }
    }

    return {};
}

bool Pathfinder::compareNodes(const Node* a, const Node* b) {
    return a->f > b->f;
}

float Pathfinder::heuristic(int x1, int y1, int x2, int y2) {
    return std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2));
}
