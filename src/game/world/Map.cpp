#include "Map.h"
#include "engine/logging/Logger.h"
#include <cstdlib>

Map::Map(int width, int height, int tileSize)
    : ground(width, height, tileSize)
    , forest(width, height, tileSize) 
    {
        blocked.assign(width * height, false);
    }

void Map::LoadGround(const std::string& tileset, int tilesPerRow, int spacing, const std::string& csv) {
    if (!ground.LoadTileset(tileset, tilesPerRow, spacing) || !ground.LoadFromCSV(csv)) {
        LOG_ERROR("Critical: failed to load ground layer. Aborting.");
        std::abort();
    }
}

void Map::LoadForest(const std::string& tileset, int tilesPerRow, int spacing, const std::string& csv) {
    if (!forest.LoadTileset(tileset, tilesPerRow, spacing) || !forest.LoadFromCSV(csv)) {
        LOG_ERROR("Critical: failed to load forest layer. Aborting.");
        std::abort();
    }
}

bool Map::IsPassable(int x, int y) const {
    if (!IsValid(x, y)) return false;
    if (blocked[y * ground.GetMapWidth() + x])
        return false;
    return forest.GetTile(x, y).id == -1;
}

void Map::SetPassable(int x, int y, bool value) {
    if (!IsValid(x, y))
        return;
    blocked[y * ground.GetMapWidth() + x] = !value;
}

bool Map::IsValid(int x, int y) const {
    return ground.IsValidPosition(x, y);
}

raylib::Vector2 Map::TileToWorld(int x, int y) const {
    return ground.TileToWorld(x, y);
}

raylib::Vector2 Map::TileCenter(int x, int y) const {
    float half = static_cast<float>(ground.GetTileSize()) * 0.5f;
    raylib::Vector2 origin = ground.TileToWorld(x, y);
    return { origin.x + half, origin.y + half };
}

raylib::Vector2 Map::WorldToTile(raylib::Vector2 world) const {
    return ground.WorldToTile(world);
}

void Map::Draw(Engine::Camera2D* camera, Engine::SpriteRenderer* renderer) {
    ground.Draw(camera, renderer);
    forest.Draw(camera, renderer);
}
