#include "Building.h"
#include "game/GameConstants.h"

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
    return {tilePos.x * TILE_SIZE, tilePos.y * TILE_SIZE};
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
            GetTileWidth()  * TILE_SIZE,
            GetTileHeight() * TILE_SIZE,
            GREEN);
    }
}

std::vector<ActionButton> Building::GetActions() const {
    switch (type) {
        case BuildingType::GreatHall:
            return {
                {ActionType::Train, 'P', "Train Peon", {55, 6, 46, 38}},
            };
        default:
            return {};
    }
}

BuildingStats Building::GetStats() const {
    BuildingStats s;
    s.hp    = hp;
    s.maxHp = maxHp;
    switch (type) {
        case BuildingType::GreatHall:
            s.name         = "Great Hall";
            s.goldCost     = 1200;
            s.lumberCost   = 0;
            s.oilCost      = 0;
            s.portraitFrame = {55, 170, 46, 38};
            break;
    }
    return s;
}