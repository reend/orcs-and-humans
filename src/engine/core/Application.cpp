#include "Application.h"
#include "Time.h"
#include "engine/logging/Logger.h"

namespace Engine {

Application* Application::instance = nullptr;

Application::Application(int width, int height, const char* title, int targetFPS)
    : running(true) {
    
    if (instance) {
        LOG_ERROR("Application already exists!");
    }
    instance = this;
    
    window = new raylib::Window(width, height, title);
    window->SetTargetFPS(targetFPS);
    
    LOG_INFO("Application created: %dx%d @ %d FPS", width, height, targetFPS);
}

Application::~Application() {
    delete window;
    LOG_INFO("Application destroyed");
}

void Application::Run() {
    LOG_INFO("Application starting main loop");
    
    while (running && !window->ShouldClose()) {
        Time::Update();
        
        BeginDrawing();
        ClearBackground(raylib::Color(30, 30, 30, 255));
        
        for (Layer* layer : layerStack) {
            layer->OnUpdate();
        }
        
        DrawFPS(window->GetWidth() - 100, 20);
        
        EndDrawing();
    }
    
    LOG_INFO("Application shutting down");
    LOG_INFO("Total runtime: %.2f seconds", Time::GetTime());
}

void Application::PushLayer(Layer* layer) {
    layerStack.PushLayer(layer);
}

void Application::PushOverlay(Layer* overlay) {
    layerStack.PushOverlay(overlay);
}

}
