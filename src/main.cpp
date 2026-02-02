#include "engine/logging/Logger.h"
#include "engine/core/Application.h"
#include "engine/core/Layer.h"
#include "engine/core/Time.h"
#include "engine/input/Input.h"
#include "engine/graphics/Camera2D.h"
#include "engine/graphics/Tilemap.h"

constexpr int WINDOW_WIDTH = 1280;
constexpr int WINDOW_HEIGHT = 720;
constexpr int TARGET_FPS = 60;

class GameLayer : public Engine::Layer {
public:
    GameLayer() : Layer("GameLayer") {}
    
    void OnAttach() override {
        LOG_INFO("GameLayer attached");
        
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
    }
    
    void OnDetach() override {
        delete forestLayer;
        delete groundLayer;
        delete camera;
    }
    
    void OnUpdate() override {
        float cameraSpeed = 400.0f * Engine::Time::GetDeltaTime();
        
        if (Engine::Input::IsKeyDown(Engine::KeyCode::W)) camera->Move({0, -cameraSpeed});
        if (Engine::Input::IsKeyDown(Engine::KeyCode::S)) camera->Move({0, cameraSpeed});
        if (Engine::Input::IsKeyDown(Engine::KeyCode::A)) camera->Move({-cameraSpeed, 0});
        if (Engine::Input::IsKeyDown(Engine::KeyCode::D)) camera->Move({cameraSpeed, 0});
        
        DrawText("Tilemap Demo", 20, 20, 30, RAYWHITE);
        DrawText("WASD - scroll map", 20, 60, 20, LIGHTGRAY);
        
        camera->Begin();
        groundLayer->Draw(camera);
        forestLayer->Draw(camera);
        camera->End();
    }
    
private:
    Engine::Camera2D* camera = nullptr;
    Engine::Tilemap* groundLayer = nullptr;
    Engine::Tilemap* forestLayer = nullptr;
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
    Engine::Logger::Get().SetMinLevel(Engine::LogLevel::INFO);
    
    LOG_INFO("=== Warcraft Engine Starting ===");
    
    WarcraftGame* game = new WarcraftGame();
    game->Run();
    delete game;
    
    return 0;
}
