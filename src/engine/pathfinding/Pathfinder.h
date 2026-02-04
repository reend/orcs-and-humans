#pragma once
#include "raylib-cpp.hpp"
#include <vector>
#include <functional>

namespace Engine {

struct PathNode {
    int x;
    int y;
    float g;
    float h;
    float f;
    PathNode* parent;
    
    PathNode(int x, int y) 
        : x(x), y(y), g(0), h(0), f(0), parent(nullptr) {}
};

class Pathfinder {
public:
    using IsPassableFunc = std::function<bool(int, int)>;
    
    static std::vector<raylib::Vector2> FindPath(
        raylib::Vector2 start,
        raylib::Vector2 goal,
        IsPassableFunc isPassable
    );
    
private:
    static float Heuristic(int x1, int y1, int x2, int y2);
    static std::vector<PathNode*> GetNeighbors(PathNode* node, int goalX, int goalY, IsPassableFunc isPassable);
    static std::vector<raylib::Vector2> ReconstructPath(PathNode* endNode);
};

}
