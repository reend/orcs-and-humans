#include "raylib-cpp.hpp"

constexpr int WINDOW_WIDTH = 1280;
constexpr int WINDOW_HEIGHT = 720;
constexpr int TARGET_FPS = 60;

int main() {
    raylib::Window window(WINDOW_WIDTH, WINDOW_HEIGHT, "Warcraft Engine");
    window.SetTargetFPS(TARGET_FPS);

    raylib::Color backgroundColor(30, 30, 30, 255);

    while (!window.ShouldClose()) {
        BeginDrawing();
        
        ClearBackground(backgroundColor);
        DrawText("Warcraft Engine v0.1", 20, 20, 40, RAYWHITE);
        DrawText("Press ESC to exit", 20, 80, 20, LIGHTGRAY);
        DrawFPS(WINDOW_WIDTH - 100, 20);
        
        EndDrawing();
    }

    return 0;
}
