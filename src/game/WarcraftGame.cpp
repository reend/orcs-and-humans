#include "WarcraftGame.h"
#include "game/GameConstants.h"

WarcraftGame::WarcraftGame()
    : Engine::Game(SCREEN_W, SCREEN_H, "Warcraft Engine", 60) {}

void WarcraftGame::OnInit() {
    world = std::make_unique<World>();
    world->Init();
    cursors[CursorType::Orc]   = LoadTexture("assets/ui/cursors/orc.png");
    cursors[CursorType::Human] = LoadTexture("assets/ui/cursors/human.png");
    HideCursor();
    hud.Init();
    ToggleFullscreen();
}

void WarcraftGame::OnUpdate(float dt) {
    world->Update(dt);
}

void WarcraftGame::OnRender() {
    world->Render();
    hud.Render(world->GetFirstSelectedUnit(), world->GetFirstSelectedBuilding());
    DrawTexture(cursors[activeCursor], GetMouseX(), GetMouseY(), WHITE);
}

void WarcraftGame::OnShutdown() {
    hud.Shutdown();
    for (auto& [type, tex] : cursors)
        UnloadTexture(tex);
    ShowCursor();
}
