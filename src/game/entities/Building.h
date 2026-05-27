#pragma once
#include "engine/graphics/Sprite.h"
#include "engine/graphics/SpriteRenderer.h"
#include "game/GameTypes.h"
#include "raylib-cpp.hpp"
#include <memory>
#include <vector> 

struct BuildingStats {
    const char* name        = "Building";
    int         hp          = 0;
    int         maxHp       = 0;
    int         goldCost    = 0;
    int         lumberCost  = 0;
    int         oilCost     = 0;
    Rectangle   portraitFrame = {};
};

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

    std::vector<ActionButton> GetActions() const;
    BuildingStats GetStats() const;

private:
    int hp    = 1200;
    int maxHp = 1200;
    raylib::Vector2   tilePos;
    BuildingType      type;
    raylib::Rectangle srcRect;

    static std::unique_ptr<Engine::Sprite> sheet;

    bool selected = false;
};