#pragma once
#include "engine/core/Game.h"
#include "game/world/World.h"
#include <memory>
#include <unordered_map>

enum class CursorType { Orc, Human };

class WarcraftGame : public Engine::Game {
public:
    WarcraftGame();

protected:
    void OnInit()           override;
    void OnUpdate(float dt) override;
    void OnRender()         override;
    void OnShutdown()       override;

private:
    std::unique_ptr<World> world;
    std::unordered_map<CursorType, Texture2D> cursors;
    CursorType activeCursor = CursorType::Orc;
};
