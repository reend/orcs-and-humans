#pragma once
#include <vector>
#include <memory>

#include "Map.h"
#include "engine/graphics/Camera2D.h"
#include "engine/graphics/SpriteRenderer.h"
#include "engine/physics/CollisionSystem.h"

#include "game/entities/Unit.h"
#include "game/entities/Building.h"
#include "raylib.h"


class World {
public:
    World();
    ~World();

    void Init();
    void Update(float dt);
    void Render();

    Unit* SpawnUnit(raylib::Vector2 worldPos);

    void SpawnBuilding(raylib::Vector2 tilePos, BuildingType type);

private:
    void MoveCamera(float dt);
    void HandleRightClick();
    void HandleLeftClick();
    void RenderDebugPath();
    void RenderHUD();
    void RenderSelectionRect();

    Map map;
    Engine::Camera2D camera;
    Engine::SpriteRenderer renderer;

    std::vector<std::unique_ptr<Unit>> units;
    std::vector<raylib::Vector2> debugPath;

    bool isDragging = false;
    raylib::Vector2 dragStart;
    raylib::Vector2 dragCurrent;
    Rectangle GetDragRect() const;

    Engine::CollisionSystem collision;

    std::vector<std::unique_ptr<Building>> buildings;

    Shader contrastShader  = {};
    int    contrastLoc     = -1;
};
