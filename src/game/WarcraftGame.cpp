#include "WarcraftGame.h"

WarcraftGame::WarcraftGame()
    : Engine::Game(1280, 720, "Warcraft Engine", 60) {}

void WarcraftGame::OnInit() {
    world = std::make_unique<World>();
    world->Init();
    cursors[CursorType::Orc]   = LoadTexture("assets/ui/cursors/orc.png");
    cursors[CursorType::Human] = LoadTexture("assets/ui/cursors/human.png");
    HideCursor();
}

void WarcraftGame::OnUpdate(float dt) {
    world->Update(dt);
}

void WarcraftGame::OnRender() {
    world->Render();
    DrawTexture(cursors[activeCursor], GetMouseX(), GetMouseY(), WHITE);
}

void WarcraftGame::OnShutdown() {
    for (auto& [type, tex] : cursors)
        UnloadTexture(tex);
    ShowCursor();
}
