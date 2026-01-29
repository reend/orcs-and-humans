#include "engine/logging/Logger.h"
#include "engine/core/Application.h"

constexpr int WINDOW_WIDTH = 1280;
constexpr int WINDOW_HEIGHT = 720;
constexpr int TARGET_FPS = 60;

class WarcraftGame : public Engine::Application {
public:
    WarcraftGame() 
        : Application(WINDOW_WIDTH, WINDOW_HEIGHT, "Warcraft Engine", TARGET_FPS) {
        
        LOG_INFO("WarcraftGame initialized");
    }
};

int main() {
    Engine::Logger::Get().SetMinLevel(Engine::LogLevel::INFO);
    
    LOG_INFO("=== Warcraft Engine Starting ===");
    
    WarcraftGame* game = new WarcraftGame();
    game->Run();
    delete game;
    
    return 0;
}
