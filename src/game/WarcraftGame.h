#pragma once
#include "engine/core/Game.h"
#include "game/world/World.h"
#include <memory>

class WarcraftGame : public Engine::Game {
public:
    WarcraftGame();

protected:
    void OnInit()          override;
    void OnUpdate(float dt) override;
    void OnRender()        override;

private:
    std::unique_ptr<World> world;
};
