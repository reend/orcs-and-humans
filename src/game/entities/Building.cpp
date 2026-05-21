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

void Building::Draw(Engine::SpriteRenderer* renderer) {
    renderer->DrawSprite(sheet->GetTexture(), srcRect, GetPosition());
}