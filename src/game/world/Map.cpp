#include "Map.h"

Map::Map(int width, int height, int tileSize)
    : ground(width, height, tileSize)
    , forest(width, height, tileSize) {}

void Map::LoadGround(const std::string& tileset, int tilesPerRow, int spacing, const std::string& csv) {
    ground.LoadTileset(tileset, tilesPerRow, spacing);
    ground.LoadFromCSV(csv);
}

void Map::LoadForest(const std::string& tileset, int tilesPerRow, int spacing, const std::string& csv) {
    forest.LoadTileset(tileset, tilesPerRow, spacing);
    forest.LoadFromCSV(csv);
}

bool Map::IsPassable(int x, int y) const {
    if (!ground.IsValidPosition(x, y)) return false;
    return forest.GetTile(x, y).id == -1;
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
