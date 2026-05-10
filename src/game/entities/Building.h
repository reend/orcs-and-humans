#pragma once
#include "engine/graphics/Sprite.h"
#include <vector>
#include <memory>

class Building {
public:
    Building(raylib::Vector2 worldpos, const char *spritePath);
    void Draw();
private:
    raylib::Vector2 position;
    std::unique_ptr<Engine::Sprite> sprite;
};