#pragma once
#include <string>

namespace Engine {

enum class LogLevel { DEBUG, INFO, WARN, FATAL };

class Logger {
public:
    static Logger& Get();

    void SetMinLevel(LogLevel level);

    template<typename... Args>
    void Debug(const char* fmt, Args... args) { Log(LogLevel::DEBUG, fmt, args...); }

    template<typename... Args>
    void Info(const char* fmt, Args... args)  { Log(LogLevel::INFO,  fmt, args...); }

    template<typename... Args>
    void Warn(const char* fmt, Args... args)  { Log(LogLevel::WARN,  fmt, args...); }

    template<typename... Args>
    void Error(const char* fmt, Args... args) { Log(LogLevel::FATAL, fmt, args...); }

    void Debug(const std::string& msg) { WriteLog(LogLevel::DEBUG, msg); }
    void Info (const std::string& msg) { WriteLog(LogLevel::INFO,  msg); }
    void Warn (const std::string& msg) { WriteLog(LogLevel::WARN,  msg); }
    void Error(const std::string& msg) { WriteLog(LogLevel::FATAL, msg); }

private:
    Logger();

    template<typename... Args>
    void Log(LogLevel level, const char* fmt, Args... args) {
        int size = snprintf(nullptr, 0, fmt, args...) + 1;
        if (size <= 0) return;
        std::string buf(size, '\0');
        snprintf(&buf[0], size, fmt, args...);
        buf.pop_back();
        WriteLog(level, buf);
    }

    void        WriteLog(LogLevel level, const std::string& msg);
    const char* LevelColor(LogLevel level) const;
    const char* LevelStr(LogLevel level)   const;

    static Logger* instance;
    LogLevel minLevel;
};

}

#define LOG_DEBUG(...) Engine::Logger::Get().Debug(__VA_ARGS__)
#define LOG_INFO(...)  Engine::Logger::Get().Info(__VA_ARGS__)
#define LOG_WARN(...)  Engine::Logger::Get().Warn(__VA_ARGS__)
#define LOG_ERROR(...) Engine::Logger::Get().Error(__VA_ARGS__)
