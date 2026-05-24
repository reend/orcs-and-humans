#pragma once
#include "engine/graphics/Sprite.h"
#include "engine/graphics/SpriteRenderer.h"
#include "raylib-cpp.hpp"
#include <memory>

enum class BuildingType
{
    GreatHall
};

class Building {
public:
    Building(raylib::Vector2 tilePos, BuildingType type);

    void Draw(Engine::SpriteRenderer *renderer);
    
    raylib::Vector2 GetPosition() const;
    int GetTileWidth() const;
    int GetTileHeight() const;

    bool IsSelected() const { return selected; }
    void SetSelected(bool value) { selected = value; }

private:
    raylib::Vector2   tilePos;
    BuildingType      type;
    raylib::Rectangle srcRect;

    static std::unique_ptr<Engine::Sprite> sheet;

    bool selected = false;
};