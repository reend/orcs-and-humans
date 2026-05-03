#include "Game.h"
#include "Time.h"
#include "engine/logging/Logger.h"
#include "raylib.h"

namespace Engine {

Game::Game(int width, int height, const char* title, int fps)
    : window(std::make_unique<raylib::Window>(width, height, title)) {
    window->SetTargetFPS(fps);
    LOG_INFO("Game created: %dx%d @ %d FPS", width, height, fps);
}

void Game::Run() {
    OnInit();
    LOG_INFO("Main loop started");

    while (!window->ShouldClose()) {
        Time::Update();
        OnUpdate(Time::GetDeltaTime());

        BeginDrawing();
        ClearBackground(BLACK);
        OnRender();
        DrawFPS(GetScreenWidth() - 100, 10);
        EndDrawing();
    }

    OnShutdown();
    LOG_INFO("Shutting down. Total time: %.2fs", Time::GetTime());
}

}
