#include "Tilemap.h"
#include "engine/logging/Logger.h"
#include <fstream>
#include <sstream>

namespace Engine {

Tilemap::Tilemap(int mapWidth, int mapHeight, int tileSize)
    : mapWidth(mapWidth)
    , mapHeight(mapHeight)
    , tileSize(tileSize)
    , tilesPerRow(0)
    , spacing(0)
    , tilesetSprite(nullptr) {
    
    tiles.resize(mapHeight);
    for (int y = 0; y < mapHeight; y++) {
        tiles[y].resize(mapWidth);
        for (int x = 0; x < mapWidth; x++) {
            tiles[y][x] = {0, true};
        }
    }
    
    LOG_DEBUG("Tilemap created: %dx%d tiles, size %d", mapWidth, mapHeight, tileSize);
}

Tilemap::~Tilemap() {
    delete tilesetSprite;
}

bool Tilemap::LoadTileset(const std::string& tilesetPath, int tilesPerRow, int spacing) {
    this->tilesPerRow = tilesPerRow;
    this->spacing = spacing;
    
    tilesetSprite = new Sprite(tilesetPath);
    
    if (!tilesetSprite->IsLoaded()) {
        LOG_ERROR("Failed to load tileset: %s", tilesetPath.c_str());
        delete tilesetSprite;
        tilesetSprite = nullptr;
        return false;
    }
    
    LOG_INFO("Tileset loaded: %s (%d tiles per row, spacing %d)", tilesetPath.c_str(), tilesPerRow, spacing);
    return true;
}

bool Tilemap::LoadFromCSV(const std::string& csvPath) {
    std::ifstream file(csvPath);
    if (!file.is_open()) {
        LOG_ERROR("Failed to open CSV file: %s", csvPath.c_str());
        return false;
    }
    
    std::vector<std::vector<int>> tempData;
    std::string line;
    
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        std::vector<int> row;
        std::stringstream ss(line);
        std::string value;
        
        while (std::getline(ss, value, ',')) {
            row.push_back(std::stoi(value));
        }
        
        if (!row.empty()) {
            tempData.push_back(row);
        }
    }
    
    file.close();
    
    if (tempData.empty()) {
        LOG_ERROR("CSV file is empty: %s", csvPath.c_str());
        return false;
    }
    
    int csvHeight = tempData.size();
    int csvWidth = tempData[0].size();
    
    if (csvWidth != mapWidth || csvHeight != mapHeight) {
        LOG_WARN("CSV size (%dx%d) differs from tilemap size (%dx%d), resizing...", 
                 csvWidth, csvHeight, mapWidth, mapHeight);
        
        mapWidth = csvWidth;
        mapHeight = csvHeight;
        tiles.resize(mapHeight);
        for (int y = 0; y < mapHeight; y++) {
            tiles[y].resize(mapWidth);
        }
    }
    
    for (int y = 0; y < mapHeight; y++) {
        for (int x = 0; x < mapWidth; x++) {
            int tileId = tempData[y][x];
            if (tileId == -1) {
                tiles[y][x].id = -1;
                tiles[y][x].passable = true;
            } else {
                tiles[y][x].id = tileId;
                tiles[y][x].passable = true;
            }
        }
    }
    
    LOG_INFO("Map loaded from CSV: %s (%dx%d)", csvPath.c_str(), mapWidth, mapHeight);
    return true;
}

void Tilemap::SetTile(int x, int y, int tileId, bool passable) {
    if (!IsValidPosition(x, y)) return;
    
    tiles[y][x].id = tileId;
    tiles[y][x].passable = passable;
}

Tile Tilemap::GetTile(int x, int y) const {
    if (!IsValidPosition(x, y)) {
        return {0, false};
    }
    return tiles[y][x];
}

bool Tilemap::IsPassable(int x, int y) const {
    if (!IsValidPosition(x, y)) return false;
    return tiles[y][x].passable;
}

bool Tilemap::IsValidPosition(int x, int y) const {
    return x >= 0 && x < mapWidth && y >= 0 && y < mapHeight;
}

raylib::Vector2 Tilemap::TileToWorld(int tileX, int tileY) const {
    return raylib::Vector2{
        static_cast<float>(tileX * tileSize),
        static_cast<float>(tileY * tileSize)
    };
}

raylib::Vector2 Tilemap::WorldToTile(raylib::Vector2 worldPos) const {
    return raylib::Vector2{
        worldPos.x / static_cast<float>(tileSize),
        worldPos.y / static_cast<float>(tileSize)
    };
}

void Tilemap::Draw(Camera2D* camera, SpriteRenderer* renderer) {
    if (!tilesetSprite || !tilesetSprite->IsLoaded()) return;
    
    raylib::Vector2 camPos = camera->GetPosition();
    int startX = static_cast<int>(camPos.x / tileSize) - 2;
    int startY = static_cast<int>(camPos.y / tileSize) - 2;
    int endX = startX + (::GetScreenWidth() / tileSize) + 4;
    int endY = startY + (::GetScreenHeight() / tileSize) + 4;
    
    if (startX < 0) startX = 0;
    if (startY < 0) startY = 0;
    if (endX > mapWidth) endX = mapWidth;
    if (endY > mapHeight) endY = mapHeight;
    
    for (int y = startY; y < endY; y++) {
        for (int x = startX; x < endX; x++) {
            int tileId = tiles[y][x].id;
            
            if (tileId == -1) continue;
            
            int srcX = (tileId % tilesPerRow) * (tileSize + spacing);
            int srcY = (tileId / tilesPerRow) * (tileSize + spacing);
            
            raylib::Rectangle sourceRect = {
                static_cast<float>(srcX),
                static_cast<float>(srcY),
                static_cast<float>(tileSize),
                static_cast<float>(tileSize)
            };
            
            raylib::Vector2 worldPos = TileToWorld(x, y);
            
            renderer->DrawSprite(
                tilesetSprite->GetTexture(),
                sourceRect,
                worldPos
            );
        }
    }
}

}
