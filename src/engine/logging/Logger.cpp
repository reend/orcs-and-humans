#include "Logger.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <windows.h>

namespace Engine {

Logger* Logger::instance = nullptr;
std::mutex Logger::mutex;

Logger::Logger() 
    : minLevel(LogLevel::DEBUG) {
    
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(hConsole, &mode);
    mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hConsole, mode);
}

Logger& Logger::Get() {
    if (!instance) {
        std::lock_guard<std::mutex> lock(mutex);
        if (!instance) {
            instance = new Logger();
        }
    }
    return *instance;
}

void Logger::SetMinLevel(LogLevel level) {
    minLevel = level;
}

std::string Logger::GetTimestamp() const {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;
    
    std::tm tm;
    localtime_s(&tm, &time);
    
    std::ostringstream oss;
    oss << std::put_time(&tm, "%H:%M:%S");
    oss << '.' << std::setfill('0') << std::setw(3) << ms.count();
    return oss.str();
}

std::string Logger::LevelToString(LogLevel level) const {
    switch (level) {
        case LogLevel::DEBUG: return "DEBUG";
        case LogLevel::INFO:  return "INFO ";
        case LogLevel::WARN:  return "WARN ";
        case LogLevel::FATAL: return "FATAL";
        default: return "UNKNOWN";
    }
}

std::string Logger::GetColorCode(LogLevel level) const {
    switch (level) {
        case LogLevel::DEBUG: return "\033[36m";
        case LogLevel::INFO:  return "\033[32m";
        case LogLevel::WARN:  return "\033[33m";
        case LogLevel::FATAL: return "\033[31m";
        default: return "\033[0m";
    }
}

void Logger::WriteLog(LogLevel level, const std::string& message) {
    if (level < minLevel) return;
    
    std::lock_guard<std::mutex> lock(mutex);
    
    std::string timestamp = GetTimestamp();
    std::string levelStr = LevelToString(level);
    std::string colorCode = GetColorCode(level);
    
    std::cout << colorCode << "[" << timestamp << "] [" 
              << levelStr << "] " << message << "\033[0m" << std::endl;
}

void Logger::Debug(const std::string& message) {
    WriteLog(LogLevel::DEBUG, message);
}

void Logger::Info(const std::string& message) {
    WriteLog(LogLevel::INFO, message);
}

void Logger::Warn(const std::string& message) {
    WriteLog(LogLevel::WARN, message);
}

void Logger::Error(const std::string& message) {
    WriteLog(LogLevel::FATAL, message);
}

}
