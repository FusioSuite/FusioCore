#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <fstream>
#include <string>
#include <mutex>

enum class LogType {
    DEFAULT,
    DEBUG,
    INFO,
    WARNING,
    ERROR
};

class Logger {
public:
    // === Singleton global ===
    static Logger& getGlobal();

    // === Constructeur normal ===
    Logger(const std::string& filePath);
    ~Logger();

    void log(const std::string& message, LogType type = LogType::DEFAULT);
    void clearLogs();

private:
    std::string filePath;
    std::ofstream file;
    std::mutex logMutex;
};

#endif // LOGGER_HPP
