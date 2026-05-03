#include "Logger.h"
#include <iostream>
#include <windows.h>

namespace Engine {

Logger* Logger::instance = nullptr;

Logger::Logger() : minLevel(LogLevel::DEBUG) {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(h, &mode);
    SetConsoleMode(h, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}

Logger& Logger::Get() {
    if (!instance) instance = new Logger();
    return *instance;
}

void Logger::SetMinLevel(LogLevel level) {
    minLevel = level;
}

const char* Logger::LevelColor(LogLevel level) const {
    switch (level) {
        case LogLevel::DEBUG: return "\033[36m";
        case LogLevel::INFO:  return "\033[32m";
        case LogLevel::WARN:  return "\033[33m";
        case LogLevel::FATAL: return "\033[31m";
        default:              return "\033[0m";
    }
}

const char* Logger::LevelStr(LogLevel level) const {
    switch (level) {
        case LogLevel::DEBUG: return "DEBUG";
        case LogLevel::INFO:  return "INFO ";
        case LogLevel::WARN:  return "WARN ";
        case LogLevel::FATAL: return "FATAL";
        default:              return "?????";
    }
}

void Logger::WriteLog(LogLevel level, const std::string& msg) {
    if (level < minLevel) return;
    std::cout << LevelColor(level) << "[" << LevelStr(level) << "] "
              << msg << "\033[0m\n";
}

}
