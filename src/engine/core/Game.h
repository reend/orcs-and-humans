#pragma once
#include <memory>
#include "raylib-cpp.hpp"

namespace Engine {

class Game {
public:
    Game(int width, int height, const char* title, int fps = 60);
    virtual ~Game() = default;

    void Run();

protected:
    virtual void OnInit() = 0;
    virtual void OnUpdate(float dt) = 0;
    virtual void OnRender() = 0;
    virtual void OnShutdown() {}

private:
    std::unique_ptr<raylib::Window> window;
};

}
