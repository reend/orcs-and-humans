#include "WarcraftGame.h"

WarcraftGame::WarcraftGame()
    : Engine::Game(1280, 720, "Warcraft Engine", 60) {}

void WarcraftGame::OnInit() {
    world = std::make_unique<World>();
    world->Init();
}

void WarcraftGame::OnUpdate(float dt) {
    world->Update(dt);
}

void WarcraftGame::OnRender() {
    world->Render();
}
