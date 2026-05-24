#include "Building.h"

std::unique_ptr<Engine::Sprite> Building::sheet;

Building::Building(raylib::Vector2 tilePos, BuildingType type)
    : tilePos(tilePos), type(type) {
        if (!sheet)
            sheet = std::make_unique<Engine::Sprite>("assets/buildings/orcs/buildings.png");
        
        switch (type) {
            case BuildingType::GreatHall:
                srcRect = raylib::Rectangle{134, 0, 126, 126};
                break;
        }
}

raylib::Vector2 Building::GetPosition() const {
    return {tilePos.x * 32, tilePos.y * 32};
}

int Building::GetTileWidth() const {
    switch(type) {
        case BuildingType::GreatHall:
            return 4;
        default:
            return 3;
    }
}

int Building::GetTileHeight() const {
    switch(type) {
        case BuildingType::GreatHall:
            return 4;
        default:
            return 3;
    }
}

void Building::Draw(Engine::SpriteRenderer* renderer) {
    renderer->DrawSprite(sheet->GetTexture(), srcRect, GetPosition());
    if (selected) {
        raylib::Vector2 pos = GetPosition();
        DrawRectangleLines(
            (int)pos.x,
            (int)pos.y,
            GetTileWidth() * 32,
            GetTileHeight() * 32,
            GREEN);
    }
}