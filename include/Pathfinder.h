#ifndef PATHFINDER_H
#define PATHFINDER_H

#include <vector>
#include <queue>
#include <cmath>
#include "Map.h"

class Pathfinder {
private:
    Map* map;
    
    struct Node {
        int x, y;
        float g, h, f;
        Node* parent;

        Node(int x, int y, Node* parent = nullptr)
            : x(x), y(y), g(0), h(0), f(0), parent(parent) {}
    };
    
    static bool compareNodes(const Node* a, const Node* b);
    float heuristic(int x1, int y1, int x2, int y2);
    
public:
    Pathfinder(Map* map);
    std::vector<std::pair<float, float>> findPath(float startX, float startY, float endX, float endY);
};

#endif
