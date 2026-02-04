#include "Pathfinder.h"
#include "engine/logging/Logger.h"
#include <cmath>
#include <algorithm>

namespace Engine {

std::vector<raylib::Vector2> Pathfinder::FindPath(
    raylib::Vector2 start,
    raylib::Vector2 goal,
    IsPassableFunc isPassable
) {
    int startX = static_cast<int>(start.x);
    int startY = static_cast<int>(start.y);
    int goalX = static_cast<int>(goal.x);
    int goalY = static_cast<int>(goal.y);
    
    if (!isPassable(startX, startY) || !isPassable(goalX, goalY)) {
        LOG_WARN("Pathfinding: start or goal is not passable");
        return {};
    }
    
    if (startX == goalX && startY == goalY) {
        return {start};
    }
    
    std::vector<PathNode*> openList;
    std::vector<PathNode*> closedList;
    
    PathNode* startNode = new PathNode(startX, startY);
    startNode->g = 0;
    startNode->h = Heuristic(startX, startY, goalX, goalY);
    startNode->f = startNode->h;
    openList.push_back(startNode);
    
    PathNode* goalNode = nullptr;
    
    while (!openList.empty()) {
        auto minIt = std::min_element(openList.begin(), openList.end(),
            [](PathNode* a, PathNode* b) { return a->f < b->f; });
        
        PathNode* current = *minIt;
        openList.erase(minIt);
        closedList.push_back(current);
        
        if (current->x == goalX && current->y == goalY) {
            goalNode = current;
            break;
        }
        
        std::vector<PathNode*> neighbors = GetNeighbors(current, goalX, goalY, isPassable);
        
        for (PathNode* neighbor : neighbors) {
            auto inClosed = std::find_if(closedList.begin(), closedList.end(),
                [neighbor](PathNode* n) { return n->x == neighbor->x && n->y == neighbor->y; });
            
            if (inClosed != closedList.end()) {
                delete neighbor;
                continue;
            }
            
            float tentativeG = current->g + 1.0f;
            
            auto inOpen = std::find_if(openList.begin(), openList.end(),
                [neighbor](PathNode* n) { return n->x == neighbor->x && n->y == neighbor->y; });
            
            if (inOpen != openList.end()) {
                if (tentativeG < (*inOpen)->g) {
                    (*inOpen)->g = tentativeG;
                    (*inOpen)->f = (*inOpen)->g + (*inOpen)->h;
                    (*inOpen)->parent = current;
                }
                delete neighbor;
            } else {
                neighbor->g = tentativeG;
                neighbor->f = neighbor->g + neighbor->h;
                neighbor->parent = current;
                openList.push_back(neighbor);
            }
        }
    }
    
    std::vector<raylib::Vector2> path;
    
    if (goalNode) {
        path = ReconstructPath(goalNode);
        LOG_INFO("Path found: %d nodes", path.size());
    } else {
        LOG_WARN("No path found from (%d,%d) to (%d,%d)", startX, startY, goalX, goalY);
    }
    
    for (PathNode* node : openList) delete node;
    for (PathNode* node : closedList) delete node;
    
    return path;
}

float Pathfinder::Heuristic(int x1, int y1, int x2, int y2) {
    return std::abs(x2 - x1) + std::abs(y2 - y1);
}

std::vector<PathNode*> Pathfinder::GetNeighbors(PathNode* node, int goalX, int goalY, IsPassableFunc isPassable) {
    std::vector<PathNode*> neighbors;
    
    const int dx[] = {0, 1, 0, -1};
    const int dy[] = {-1, 0, 1, 0};
    
    for (int i = 0; i < 4; i++) {
        int nx = node->x + dx[i];
        int ny = node->y + dy[i];
        
        if (isPassable(nx, ny)) {
            PathNode* neighbor = new PathNode(nx, ny);
            neighbor->h = Heuristic(nx, ny, goalX, goalY);
            neighbors.push_back(neighbor);
        }
    }
    
    return neighbors;
}

std::vector<raylib::Vector2> Pathfinder::ReconstructPath(PathNode* endNode) {
    std::vector<raylib::Vector2> path;
    PathNode* current = endNode;
    
    while (current != nullptr) {
        path.push_back(raylib::Vector2(static_cast<float>(current->x), static_cast<float>(current->y)));
        current = current->parent;
    }
    
    std::reverse(path.begin(), path.end());
    return path;
}

}
