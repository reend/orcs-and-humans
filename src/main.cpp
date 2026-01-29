#include "raylib-cpp.hpp"
#include "engine/logging/Logger.h"
#include "engine/core/Time.h"
#include "engine/input/Input.h"

constexpr int WINDOW_WIDTH = 1280;
constexpr int WINDOW_HEIGHT = 720;
constexpr int TARGET_FPS = 60;

int main() {
    Engine::Logger::Get().SetMinLevel(Engine::LogLevel::INFO);
    
    LOG_INFO("=== Warcraft Engine Starting ===");
    LOG_INFO("Window size: %dx%d", WINDOW_WIDTH, WINDOW_HEIGHT);
    LOG_INFO("Target FPS: %d", TARGET_FPS);
    
    raylib::Window window(WINDOW_WIDTH, WINDOW_HEIGHT, "Warcraft Engine");
    window.SetTargetFPS(TARGET_FPS);
    
    LOG_INFO("Window created successfully");
    LOG_INFO("Time system initialized");
    LOG_INFO("Input system initialized");
    
    raylib::Color backgroundColor(30, 30, 30, 255);
    float testX = 640.0f;
    float testY = 360.0f;
    constexpr float moveSpeed = 300.0f;

    while (!window.ShouldClose()) {
        Engine::Time::Update();
        
        if (Engine::Input::IsKeyDown(Engine::KeyCode::W)) {
            testY -= moveSpeed * Engine::Time::GetDeltaTime();
        }
        if (Engine::Input::IsKeyDown(Engine::KeyCode::S)) {
            testY += moveSpeed * Engine::Time::GetDeltaTime();
        }
        if (Engine::Input::IsKeyDown(Engine::KeyCode::A)) {
            testX -= moveSpeed * Engine::Time::GetDeltaTime();
        }
        if (Engine::Input::IsKeyDown(Engine::KeyCode::D)) {
            testX += moveSpeed * Engine::Time::GetDeltaTime();
        }
        
        if (Engine::Input::IsMouseButtonPressed(Engine::MouseButton::Left)) {
            Engine::Vector2 mousePos = Engine::Input::GetMousePosition();
            LOG_INFO("Mouse clicked at: %.0f, %.0f", mousePos.x, mousePos.y);
        }
        
        BeginDrawing();
        ClearBackground(backgroundColor);
        
        DrawText("Warcraft Engine v0.1", 20, 20, 40, RAYWHITE);
        DrawText("Press ESC to exit", 20, 80, 20, LIGHTGRAY);
        DrawText("WASD to move circle, click mouse", 20, 120, 20, LIGHTGRAY);
        DrawFPS(WINDOW_WIDTH - 100, 20);
        
        DrawCircle(static_cast<int>(testX), static_cast<int>(testY), 30, RED);
        
        EndDrawing();
    }
    
    LOG_INFO("Shutting down engine");
    LOG_INFO("Total runtime: %.2f seconds", Engine::Time::GetTime());

    return 0;
}
