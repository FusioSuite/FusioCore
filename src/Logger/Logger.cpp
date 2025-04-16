#include "Logger/Logger.hpp"
#include <chrono>
#include <ctime>
#include <iostream>

Logger& Logger::getInstance() {
    static Logger instance("fusio_log.txt");
    return instance;
}

Logger::Logger(const std::string& filePath) : filePath(filePath) {
    file.open(filePath, std::ios::app);
}

Logger::~Logger() {
    if (file.is_open()) file.close();
}

void Logger::log(const std::string& message, LogType type) {
    std::lock_guard<std::mutex> lock(logMutex);
    if (file.is_open()) {
        // Get current time
        auto now = std::chrono::system_clock::now();
        std::time_t t = std::chrono::system_clock::to_time_t(now);
        char buffer[64];
        std::strftime(buffer, sizeof(buffer), "[%Y-%m-%d %H:%M:%S]", std::localtime(&t));

        // Get log type
        std::string logType;
        switch (type) {
            case LogType::DEBUG:
                logType = "[DEBUG] | " + std::string(buffer) + " | ";
                break;
            case LogType::INFO:
                logType = "[INFO] | " + std::string(buffer) + " | ";
                break;
            case LogType::WARNING:
                logType = "[WARNING] | " + std::string(buffer) + " | ";
                break;
            case LogType::ERROR:
                logType = "[ERROR] | " + std::string(buffer) + " | ";
                break;
            default:
                logType = "";
                break;
        }
        file << logType << message << std::endl;
    }
}

void Logger::clearLogs() {
    std::lock_guard<std::mutex> lock(logMutex);
    if (file.is_open()) {
        file.close();
    }
    file.open(filePath, std::ios::trunc);
    file.close();
    file.open(filePath, std::ios::app);
}