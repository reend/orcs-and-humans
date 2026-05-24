#pragma once
#include "engine/graphics/Tilemap.h"
#include "engine/graphics/Camera2D.h"
#include "engine/graphics/SpriteRenderer.h"
#include <string>

class Map {
public:
    Map(int width, int height, int tileSize);

    void LoadGround(const std::string& tileset, int tilesPerRow, int spacing, const std::string& csv);
    void LoadForest(const std::string& tileset, int tilesPerRow, int spacing, const std::string& csv);

    // Forest tile present = impassable (tree blocking)
    bool IsPassable(int x, int y) const;
    bool IsValid(int x, int y) const;

    raylib::Vector2 TileToWorld(int x, int y) const;
    raylib::Vector2 TileCenter(int x, int y) const;
    raylib::Vector2 WorldToTile(raylib::Vector2 world) const;

    void Draw(Engine::Camera2D* camera, Engine::SpriteRenderer* renderer);

    int GetWidth()    const { return ground.GetMapWidth();  }
    int GetHeight()   const { return ground.GetMapHeight(); }
    int GetTileSize() const { return ground.GetTileSize();  }

    void SetPassable(int x, int y, bool value);

private:
    Engine::Tilemap ground;
    Engine::Tilemap forest;
    std::vector<bool> blocked;
};
