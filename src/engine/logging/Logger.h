#pragma once
#include <string>
#include <sstream>
#include <mutex>

namespace Engine {

enum class LogLevel {
    DEBUG,
    INFO,
    WARN,
    FATAL
};

class Logger {
private:
    static Logger* instance;
    static std::mutex mutex;
    
    LogLevel minLevel;
    
    Logger();
    ~Logger() = default;
    
    std::string GetTimestamp() const;
    std::string LevelToString(LogLevel level) const;
    std::string GetColorCode(LogLevel level) const;
    void WriteLog(LogLevel level, const std::string& message);

public:
    static Logger& Get();
    
    void SetMinLevel(LogLevel level);
    
    void Debug(const std::string& message);
    void Info(const std::string& message);
    void Warn(const std::string& message);
    void Error(const std::string& message);
    
    template<typename... Args>
    void Debug(const char* format, Args... args) {
        Debug(Format(format, args...));
    }
    
    template<typename... Args>
    void Info(const char* format, Args... args) {
        Info(Format(format, args...));
    }
    
    template<typename... Args>
    void Warn(const char* format, Args... args) {
        Warn(Format(format, args...));
    }
    
    template<typename... Args>
    void Error(const char* format, Args... args) {
        Error(Format(format, args...));
    }

private:
    template<typename... Args>
    std::string Format(const char* format, Args... args) {
        int size = snprintf(nullptr, 0, format, args...) + 1;
        if (size <= 0) return "";
        
        std::string buffer(size, '\0');
        snprintf(&buffer[0], size, format, args...);
        buffer.pop_back();
        return buffer;
    }
};

}

#define LOG_DEBUG(...) Engine::Logger::Get().Debug(__VA_ARGS__)
#define LOG_INFO(...) Engine::Logger::Get().Info(__VA_ARGS__)
#define LOG_WARN(...) Engine::Logger::Get().Warn(__VA_ARGS__)
#define LOG_ERROR(...) Engine::Logger::Get().Error(__VA_ARGS__)
