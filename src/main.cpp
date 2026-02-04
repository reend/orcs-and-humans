#include "engine/logging/Logger.h"
#include "engine/core/Application.h"
#include "engine/core/Layer.h"
#include "engine/core/Time.h"
#include "engine/input/Input.h"
#include "engine/graphics/Camera2D.h"
#include "engine/graphics/Tilemap.h"
#include "engine/graphics/SpriteRenderer.h"
#include "engine/pathfinding/Pathfinder.h"
#include "game/entities/Unit.h"

constexpr int WINDOW_WIDTH = 1280;
constexpr int WINDOW_HEIGHT = 720;
constexpr int TARGET_FPS = 60;

class GameLayer : public Engine::Layer {
public:
    GameLayer() : Layer("GameLayer") {}
    
    void OnAttach() override {
        LOG_INFO("GameLayer attached");
        
        renderer = new Engine::SpriteRenderer();
        
        camera = new Engine::Camera2D(0, 0);
        camera->SetZoom(1.5f);
        camera->SetBounds(0, 0, 960, 640);
        camera->EnableBounds(true);
        
        groundLayer = new Engine::Tilemap(30, 20, 32);
        groundLayer->LoadTileset("assets/winter/tileset.png", 19, 1);
        groundLayer->LoadFromCSV("assets/winter/ground.csv");
        
        forestLayer = new Engine::Tilemap(30, 20, 32);
        forestLayer->LoadTileset("assets/winter/tileset.png", 19, 1);
        forestLayer->LoadFromCSV("assets/winter/forest.csv");

        LOG_INFO("Tilemap layers initialized (ground + forest)");

        raylib::Vector2 unitStartPos = groundLayer->TileToWorld(5, 5);
        unitStartPos.x += 16;
        unitStartPos.y += 16;
        unit = new Unit(unitStartPos);

        LOG_INFO("Unit created at tile (5,5)");
    }
    
    void OnDetach() override {
        delete unit;
        delete forestLayer;
        delete groundLayer;
        delete camera;
        delete renderer;
    }
    
    void OnUpdate() override {
        float cameraSpeed = 400.0f * Engine::Time::GetDeltaTime();
        
        if (Engine::Input::IsKeyDown(Engine::KeyCode::W)) camera->Move({0, -cameraSpeed});
        if (Engine::Input::IsKeyDown(Engine::KeyCode::S)) camera->Move({0, cameraSpeed});
        if (Engine::Input::IsKeyDown(Engine::KeyCode::A)) camera->Move({-cameraSpeed, 0});
        if (Engine::Input::IsKeyDown(Engine::KeyCode::D)) camera->Move({cameraSpeed, 0});
        
        unit->Update(Engine::Time::GetDeltaTime());

        if (Engine::Input::IsMouseButtonPressed(Engine::MouseButton::Right)) {
            Engine::Vector2 mousePos = Engine::Input::GetMousePosition();
            raylib::Vector2 worldPos = camera->ScreenToWorld(raylib::Vector2(mousePos.x, mousePos.y));
            raylib::Vector2 tilePos = groundLayer->WorldToTile(worldPos);

            raylib::Vector2 unitTilePos = groundLayer->WorldToTile(unit->GetPosition());

            std::vector<raylib::Vector2> path = Engine::Pathfinder::FindPath(
                unitTilePos,
                tilePos,
                [this](int x, int y) {
                    return forestLayer->IsPassable(x, y);
                }
            );

            if (!path.empty()) {
                std::vector<raylib::Vector2> worldPath;
                for (const auto& tilePosInPath : path) {
                    raylib::Vector2 worldPosInPath = groundLayer->TileToWorld(
                        static_cast<int>(tilePosInPath.x),
                        static_cast<int>(tilePosInPath.y)
                    );
                    worldPosInPath.x += 16;
                    worldPosInPath.y += 16;
                    worldPath.push_back(worldPosInPath);
                }
                LOG_INFO("Setting unit path with %d waypoints (from tile %.0f,%.0f to %.0f,%.0f)",
                         worldPath.size(), unitTilePos.x, unitTilePos.y, tilePos.x, tilePos.y);
                unit->SetPath(worldPath);
                currentPath = path;
            }
        }
        
        camera->Begin();
        renderer->Begin();
        groundLayer->Draw(camera, renderer);
        forestLayer->Draw(camera, renderer);
        renderer->End();

        unit->Draw();

        if (!currentPath.empty()) {
            for (size_t i = 0; i < currentPath.size() - 1; i++) {
                raylib::Vector2 p1 = groundLayer->TileToWorld(
                    static_cast<int>(currentPath[i].x),
                    static_cast<int>(currentPath[i].y)
                );
                raylib::Vector2 p2 = groundLayer->TileToWorld(
                    static_cast<int>(currentPath[i + 1].x),
                    static_cast<int>(currentPath[i + 1].y)
                );
                
                p1.x += 16;
                p1.y += 16;
                p2.x += 16;
                p2.y += 16;
                
                DrawLineEx(p1, p2, 2.0f, Fade(YELLOW, 0.3f));  // Полупрозрачная желтая линия (30%)
            }

            for (const auto& node : currentPath) {
                raylib::Vector2 worldPos = groundLayer->TileToWorld(
                    static_cast<int>(node.x),
                    static_cast<int>(node.y)
                );
                worldPos.x += 16;
                worldPos.y += 16;
                DrawCircle(static_cast<int>(worldPos.x), static_cast<int>(worldPos.y), 3, Fade(RED, 0.4f));  // Полупрозрачные красные точки (40%)
            }
        }
        
        camera->End();
        
        DrawText("Unit System Demo", 20, 20, 30, RAYWHITE);
        DrawText("WASD - scroll | Right Click - move unit", 20, 60, 20, LIGHTGRAY);
        
        char statsText[128];
        sprintf(statsText, "Batches: %d | Draw Calls: %d | Path nodes: %d", 
                renderer->GetBatchCount(), renderer->GetDrawCallCount(), currentPath.size());
        DrawText(statsText, 20, 100, 20, LIME);
    }
    
private:
    Engine::Camera2D* camera = nullptr;
    Engine::Tilemap* groundLayer = nullptr;
    Engine::Tilemap* forestLayer = nullptr;
    Engine::SpriteRenderer* renderer = nullptr;
    Unit* unit = nullptr;
    std::vector<raylib::Vector2> currentPath;
};

class WarcraftGame : public Engine::Application {
public:
    WarcraftGame() 
        : Application(WINDOW_WIDTH, WINDOW_HEIGHT, "Warcraft Engine", TARGET_FPS) {
        
        PushLayer(new GameLayer());
        
        LOG_INFO("WarcraftGame initialized");
    }
};

int main() {
    Engine::Logger::Get().SetMinLevel(Engine::LogLevel::DEBUG);  // DEBUG для видимости всех логов

    LOG_INFO("=== Warcraft Engine Starting ===");
    
    WarcraftGame* game = new WarcraftGame();
    game->Run();
    delete game;
    
    return 0;
}
