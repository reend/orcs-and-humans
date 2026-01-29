#pragma once
#include "LayerStack.h"
#include "raylib-cpp.hpp"

namespace Engine {

class Application {
public:
    Application(int width, int height, const char* title, int targetFPS = 60);
    virtual ~Application();

    void Run();
    
    void PushLayer(Layer* layer);
    void PushOverlay(Layer* overlay);

    static Application& Get() { return *instance; }
    
    raylib::Window& GetWindow() { return *window; }

protected:
    raylib::Window* window;
    LayerStack layerStack;
    bool running;
    
private:
    static Application* instance;
};

}
