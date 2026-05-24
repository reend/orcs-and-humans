#include "World.h"
#include "engine/core/Time.h"
#include "engine/input/Input.h"
#include "engine/pathfinding/Pathfinder.h"
#include "engine/logging/Logger.h"
#include "game/GameConstants.h"
#include "raylib.h"
#include <algorithm>

static const raylib::Vector2 OFFSETS[] = {
    {0,0}, {1,0}, {-1,0}, {0,1}, {0,-1}, {1,1}, {-1,1}, {1,-1}, {-1,-1}
};

World::World()
    : map(128, 128, 32)
    , camera(0.0f, 0.0f) {}

World::~World() {
    UnloadShader(contrastShader);
}

void World::Init() {
    map.LoadGround("assets/winter/tileset.png", 19, 1, "assets/winter/ground.csv");
    map.LoadForest("assets/winter/tileset.png", 19, 1, "assets/winter/forest.csv");

    contrastShader = LoadShader(0, "assets/shaders/contrast.frag");
    contrastLoc    = GetShaderLocation(contrastShader, "contrast");
    float contrast = 1.1f;
    SetShaderValue(contrastShader, contrastLoc, &contrast, SHADER_UNIFORM_FLOAT);

    camera.SetZoom(2.1f);
    camera.SetOffset({HUD_W, 0});
    camera.SetBounds(0, 0, 128 * 32, 128 * 32);
    camera.EnableBounds(true);

    SpawnUnit(map.TileCenter(5, 5));
    SpawnUnit(map.TileCenter(7, 5));

    SpawnBuilding({10, 10}, BuildingType::GreatHall);
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
    HandleLeftClick();
    HandleRightClick();
    for (auto& unit : units)
        unit->Update(dt);
    collision.Resolve(units);
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

    int slot = 0;
    for (auto& unit : units) {
        if (!unit->IsSelected()) continue;
        raylib::Vector2 unitTile = map.WorldToTile(unit->GetPosition());

        raylib::Vector2 target = { tilePos.x + OFFSETS[slot % 9].x,
            tilePos.y + OFFSETS[slot % 9].y };
        slot++;

        auto path = Engine::Pathfinder::FindPath(unitTile, target,
            [this](int x, int y) { return map.IsPassable(x, y); });

        if (path.empty()) continue;

        std::vector<raylib::Vector2> worldPath;
        worldPath.reserve(path.size());
        for (const auto& t : path)
            worldPath.push_back(map.TileCenter((int)t.x, (int)t.y));

        unit->SetPath(worldPath);
        debugPath = path;
        LOG_DEBUG("Path set: %d waypoints -> tile (%.0f, %.0f)", (int)path.size(), tilePos.x, tilePos.y);
    }
}

Rectangle World::GetDragRect() const {
    return {
        std::min(dragStart.x, dragCurrent.x),
        std::min(dragStart.y, dragCurrent.y),
        std::abs(dragCurrent.x - dragStart.x),
        std::abs(dragCurrent.y - dragStart.y)
    };
}

void World::RenderSelectionRect() {
    if (!isDragging) return;
    DrawRectangleLinesEx(GetDragRect(), 2.0f, GREEN);
}

void World::HandleLeftClick() {
    if (Engine::Input::IsMouseButtonPressed(Engine::MouseButton::Left)) {
        dragStart = Engine::Input::GetMousePosition();
        isDragging = true;
    }

    if (isDragging) {
        dragCurrent = Engine::Input::GetMousePosition();
    }

    if (Engine::Input::IsMouseButtonReleased(Engine::MouseButton::Left)) {
        isDragging = false;
        Rectangle selRect = GetDragRect();

        bool isClick = selRect.width < 5 && selRect.height < 5;

        if (isClick) {
            raylib::Vector2 mouse = Engine::Input::GetMousePosition();
            for (auto& unit : units) 
            {
                raylib::Vector2 screenPos = camera.WorldToScreen(unit->GetPosition());
                float dx = mouse.x - screenPos.x;
                float dy = mouse.y - screenPos.y;
                float dist2 = (dx * dx + dy * dy);
                bool hit = dist2 < (60*60);
                unit->SetSelected(hit);
            }
            for (auto& b : buildings) {
                raylib::Vector2 worldPos = b->GetPosition();
                float bw = b->GetTileWidth()  * 32.0f;
                float bh = b->GetTileHeight() * 32.0f;
                raylib::Vector2 tl = camera.WorldToScreen(worldPos);
                Rectangle bRect = { tl.x, tl.y, bw * camera.GetZoom(), bh * camera.GetZoom() };
                b->SetSelected(CheckCollisionPointRec(mouse, bRect));            }
    }   else {
            for (auto& unit : units)
            {
                raylib::Vector2 screenPos = camera.WorldToScreen(unit->GetPosition());
                unit->SetSelected(CheckCollisionPointRec(screenPos, selRect));        
            }
            for (auto& b : buildings)
            {
                raylib::Vector2 screenPos = camera.WorldToScreen(b->GetPosition());
                b->SetSelected(CheckCollisionPointRec(screenPos, selRect));
            }
        }
    }
}

void World::SpawnBuilding(raylib::Vector2 tilePos, BuildingType type) {
    auto b = std::make_unique<Building>(tilePos, type);
    int w = b->GetTileWidth();
    int h = b->GetTileHeight();
    for (int dy = 0; dy < h; dy++)
        for (int dx = 0; dx < w; dx++)
            map.SetPassable((int)tilePos.x + dx, (int)tilePos.y + dy, false);
    buildings.push_back(std::move(b));
}

void World::Render() {
    camera.Begin();

    BeginShaderMode(contrastShader);
    renderer.Begin();
    map.Draw(&camera, &renderer);
    renderer.End();
    EndShaderMode();

    using DrawEntry = std::pair<float, std::function<void()>>;
    std::vector<DrawEntry> drawList;

    for (auto& u : units)
        drawList.push_back({u->GetPosition().y, [u = u.get()]{ u->Draw(); }});
    
    for (auto& b : buildings)
        drawList.push_back({b->GetPosition().y, [b = b.get(), this]{ b->Draw(&renderer); }});

    std::sort(drawList.begin(), drawList.end(),
              [](const DrawEntry &a, const DrawEntry &b)
              { return a.first < b.first; });

    for (auto& u : units) u->DrawShadow();

    BeginShaderMode(contrastShader);
    renderer.Begin();
    for (auto& [y, draw] : drawList) draw();
    renderer.End();
    EndShaderMode();

    camera.End();

    RenderHUD();
}

void World::RenderHUD() {
    RenderSelectionRect();

    char stats[128];
    std::snprintf(stats, sizeof(stats), "Batches: %d | Draw Calls: %d | Path: %d nodes",
        renderer.GetBatchCount(), renderer.GetDrawCallCount(), (int)debugPath.size());
    DrawText(stats, 20, 50, 18, LIME);
}
