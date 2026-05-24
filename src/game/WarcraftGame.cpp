#include "WarcraftGame.h"
#include "game/GameConstants.h"

WarcraftGame::WarcraftGame()
    : Engine::Game(1920, 1080, "Warcraft Engine", 60) {}

void WarcraftGame::OnInit() {
    world = std::make_unique<World>();
    world->Init();
    cursors[CursorType::Orc]   = LoadTexture("assets/ui/cursors/orc.png");
    cursors[CursorType::Human] = LoadTexture("assets/ui/cursors/human.png");
    HideCursor();
    hudPanel = LoadTexture("assets/ui/orcs/bg/BG_Orc.png");
    ToggleFullscreen();
}

void WarcraftGame::OnUpdate(float dt) {
    world->Update(dt);
}

void WarcraftGame::OnRender() {
    world->Render();
    Rectangle src  = { 1, 1, HUD_SRC_W, HUD_SRC_H };
    Rectangle dest = { 0, 0, HUD_W, SCREEN_H };
    DrawTexturePro(hudPanel, src, dest, {0, 0}, 0.0f, WHITE);
    DrawTexture(cursors[activeCursor], GetMouseX(), GetMouseY(), WHITE);
}

void WarcraftGame::OnShutdown() {
    UnloadTexture(hudPanel);
    for (auto& [type, tex] : cursors)
        UnloadTexture(tex);
    ShowCursor();
}
