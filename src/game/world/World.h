#pragma once
#include <vector>
#include <memory>
#include "Map.h"
#include "engine/graphics/Camera2D.h"
#include "engine/graphics/SpriteRenderer.h"
#include "game/entities/Unit.h"

class World {
public:
    World();

    void Init();
    void Update(float dt);
    void Render();

    Unit* SpawnUnit(raylib::Vector2 worldPos);

private:
    void MoveCamera(float dt);
    void HandleRightClick();
    void RenderDebugPath();
    void RenderHUD();

    Map map;
    Engine::Camera2D camera;
    Engine::SpriteRenderer renderer;

    std::vector<std::unique_ptr<Unit>> units;
    std::vector<raylib::Vector2> debugPath;
};
