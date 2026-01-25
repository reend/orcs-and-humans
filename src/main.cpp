#include "raylib-cpp.hpp"
#include "engine/logging/Logger.h"

constexpr int WINDOW_WIDTH = 1280;
constexpr int WINDOW_HEIGHT = 720;
constexpr int TARGET_FPS = 60;

int main() {
    Engine::Logger::Get().SetMinLevel(Engine::LogLevel::DEBUG);
    
    LOG_INFO("=== Warcraft Engine Starting ===");
    LOG_INFO("Window size: %dx%d", WINDOW_WIDTH, WINDOW_HEIGHT);
    LOG_INFO("Target FPS: %d", TARGET_FPS);
    
    raylib::Window window(WINDOW_WIDTH, WINDOW_HEIGHT, "Warcraft Engine");
    window.SetTargetFPS(TARGET_FPS);
    
    LOG_INFO("Window created successfully");
    
    raylib::Color backgroundColor(30, 30, 30, 255);
    int frameCount = 0;

    while (!window.ShouldClose()) {
        frameCount++;
        
        if (frameCount % 60 == 0) {
            LOG_DEBUG("Frame: %d, FPS: %d", frameCount, GetFPS());
        }
        
        BeginDrawing();
        ClearBackground(backgroundColor);
        
        DrawText("Warcraft Engine v0.1", 20, 20, 40, RAYWHITE);
        DrawText("Press ESC to exit", 20, 80, 20, LIGHTGRAY);
        DrawFPS(WINDOW_WIDTH - 100, 20);
        
        EndDrawing();
    }
    
    LOG_INFO("Shutting down engine");
    LOG_INFO("Total frames rendered: %d", frameCount);

    return 0;
}
