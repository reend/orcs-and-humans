#pragma once
#include "Sprite.h"
#include "Camera2D.h"
#include <vector>

namespace Engine {

struct Tile {
    int id;
    bool passable;
};

class Tilemap {
public:
    Tilemap(int mapWidth, int mapHeight, int tileSize);
    ~Tilemap();
    
    bool LoadTileset(const std::string& tilesetPath, int tilesPerRow, int spacing = 0);
    bool LoadFromCSV(const std::string& csvPath);
    
    void SetTile(int x, int y, int tileId, bool passable = true);
    Tile GetTile(int x, int y) const;
    
    bool IsPassable(int x, int y) const;
    bool IsValidPosition(int x, int y) const;
    
    raylib::Vector2 TileToWorld(int tileX, int tileY) const;
    raylib::Vector2 WorldToTile(raylib::Vector2 worldPos) const;
    
    void Draw(Camera2D* camera);
    
    int GetMapWidth() const { return mapWidth; }
    int GetMapHeight() const { return mapHeight; }
    int GetTileSize() const { return tileSize; }
    
private:
    std::vector<std::vector<Tile>> tiles;
    Sprite* tilesetSprite;
    
    int mapWidth;
    int mapHeight;
    int tileSize;
    int tilesPerRow;
    int spacing;
};

}
