#pragma once
#include "raylib-cpp.hpp"
#include <vector>
#include <functional>

namespace Engine {

class Pathfinder {
public:
    using IsPassableFunc = std::function<bool(int, int)>;

    static std::vector<raylib::Vector2> FindPath(
        raylib::Vector2 start,
        raylib::Vector2 goal,
        IsPassableFunc isPassable
    );
};

}
