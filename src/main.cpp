#include "engine/logging/Logger.h"
#include "game/WarcraftGame.h"

int main() {
    Engine::Logger::Get().SetMinLevel(Engine::LogLevel::DEBUG);
    LOG_INFO("=== Warcraft Engine Starting ===");

    WarcraftGame game;
    game.Run();

    return 0;
}
