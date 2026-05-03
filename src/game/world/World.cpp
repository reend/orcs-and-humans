#include "World.h"
#include "engine/core/Time.h"
#include "engine/input/Input.h"
#include "engine/pathfinding/Pathfinder.h"
#include "engine/logging/Logger.h"
#include "raylib.h"

static constexpr float CAM_SPEED = 400.0f;

World::World()
    : map(30, 20, 32)
    , camera(0.0f, 0.0f) {}

void World::Init() {
    map.LoadGround("assets/winter/tileset.png", 19, 1, "assets/winter/ground.csv");
    map.LoadForest("assets/winter/tileset.png", 19, 1, "assets/winter/forest.csv");

    camera.SetZoom(1.5f);
    camera.SetBounds(0, 0, 960, 640);
    camera.EnableBounds(true);

    SpawnUnit(map.TileCenter(5, 5));
    LOG_INFO("World initialized");
}

Unit* World::SpawnUnit(raylib::Vector2 worldPos) {
    auto unit = std::make_unique<Unit>(worldPos);
    Unit* ptr = unit.get();
    units.push_back(std::move(unit));
    LOG_DEBUG("Unit spawned at (%.1f, %.1f)", worldPos.x, worldPos.y);
    return ptr;
}

void World::Update(float dt) {
    MoveCamera(dt);
    HandleRightClick();
    for (auto& unit : units)
        unit->Update(dt);
}

void World::MoveCamera(float dt) {
    float speed = CAM_SPEED * dt;
    if (Engine::Input::IsKeyDown(Engine::KeyCode::W)) camera.Move({0,     -speed});
    if (Engine::Input::IsKeyDown(Engine::KeyCode::S)) camera.Move({0,      speed});
    if (Engine::Input::IsKeyDown(Engine::KeyCode::A)) camera.Move({-speed, 0    });
    if (Engine::Input::IsKeyDown(Engine::KeyCode::D)) camera.Move({ speed, 0    });
}

void World::HandleRightClick() {
    if (!Engine::Input::IsMouseButtonPressed(Engine::MouseButton::Right)) return;
    if (units.empty()) return;

    raylib::Vector2 worldPos = camera.ScreenToWorld(Engine::Input::GetMousePosition());
    raylib::Vector2 tilePos  = map.WorldToTile(worldPos);

    Unit* unit = units[0].get();
    raylib::Vector2 unitTile = map.WorldToTile(unit->GetPosition());

    auto path = Engine::Pathfinder::FindPath(unitTile, tilePos,
        [this](int x, int y) { return map.IsPassable(x, y); });

    if (path.empty()) return;

    std::vector<raylib::Vector2> worldPath;
    worldPath.reserve(path.size());
    for (const auto& t : path)
        worldPath.push_back(map.TileCenter((int)t.x, (int)t.y));

    unit->SetPath(worldPath);
    debugPath = path;
    LOG_DEBUG("Path set: %d waypoints -> tile (%.0f, %.0f)", (int)path.size(), tilePos.x, tilePos.y);
}

void World::Render() {
    camera.Begin();

    renderer.Begin();
    map.Draw(&camera, &renderer);
    renderer.End();

    for (auto& unit : units)
        unit->Draw();

    RenderDebugPath();
    camera.End();

    RenderHUD();
}

void World::RenderDebugPath() {
    for (size_t i = 0; i + 1 < debugPath.size(); i++) {
        raylib::Vector2 p1 = map.TileCenter((int)debugPath[i].x,     (int)debugPath[i].y);
        raylib::Vector2 p2 = map.TileCenter((int)debugPath[i + 1].x, (int)debugPath[i + 1].y);
        DrawLineEx(p1, p2, 2.0f, Fade(YELLOW, 0.3f));
    }
    for (const auto& node : debugPath) {
        raylib::Vector2 c = map.TileCenter((int)node.x, (int)node.y);
        DrawCircle((int)c.x, (int)c.y, 3.0f, Fade(RED, 0.4f));
    }
}

void World::RenderHUD() {
    DrawText("WASD - scroll | Right Click - move unit", 20, 20, 20, LIGHTGRAY);

    char stats[128];
    std::snprintf(stats, sizeof(stats), "Batches: %d | Draw Calls: %d | Path: %d nodes",
        renderer.GetBatchCount(), renderer.GetDrawCallCount(), (int)debugPath.size());
    DrawText(stats, 20, 50, 18, LIME);
}
