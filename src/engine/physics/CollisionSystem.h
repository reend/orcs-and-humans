#pragma once
#include "game/entities/Unit.h"
#include <vector>
#include <memory>

namespace Engine {

class CollisionSystem {
public:
    void Resolve(std::vector<std::unique_ptr<Unit>> &units);

private:
    static constexpr float MIN_DIST = 25.0f;
};

}