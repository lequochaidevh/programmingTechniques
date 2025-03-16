
//g++ Logger.cpp main.cpp -o logger_app -pthread


#include "Logger.h"
#include <csignal>

void signalHandler(int signal) {
    LogError logger;
    logger.critical("Segmentation fault occurred");
    logger.flush();
    exit(signal);
}

void checkPointerInt(int *checkVar) {
    LogError logger;
    if (checkVar == nullptr) {
        logger.warning("Null pointer detected"); // default format
        logger.flush();
        throw std::runtime_error("Null pointer access attempted");
    }
}

int main() {
    LogError logger("app_log.log", true, LogError::LogLevel::INFO);
    signal(SIGSEGV, signalHandler);

    logger.setFormatter([](LogError::LogLevel level, const std::string& msg, 
                          const char* file, const char* func, int line) -> std::string {
        std::string levelStr;
        switch (level) {
            case LogError::LogLevel::INFO: levelStr = "INFO"; break;
            case LogError::LogLevel::WARNING: levelStr = "WARNING"; break;
            case LogError::LogLevel::ERROR: levelStr = "ERROR"; break;
            default: levelStr = "UNKNOWN"; break;
        }
        return "[" + levelStr + "] " + std::string(file) + ":" +
               std::string(func) + ":" + std::to_string(line) + " - " + msg;
    });

    logger.info("Application started with custom formatter");

    logger.setMaxFileSize(1024);
    for (int i = 0; i < 100; ++i) {
        logger.info("Log entry " + std::to_string(i));
    }

    logger.setMinimumLevel(LogError::LogLevel::WARNING);
    LOG_IF_LEVEL(logger, LogError::LogLevel::INFO, 
                "This won't be logged due to compile-time check");
    LOG_IF_LEVEL(logger, LogError::LogLevel::WARNING, 
                "This will be logged");

    try {
        int divisor = 0;
        if (divisor == 0) {
            logger.warning("Potential division by zero detected");
            throw std::runtime_error("Division by zero attempted");
        }
        int result = 10 / divisor;
    }
    catch (const std::exception& e) {
        logger.error(e.what());
    }

    try {
        int* ptr = nullptr;
        checkPointerInt(ptr);
        *ptr = 42;
    }
    catch (const std::exception& e) {
        logger.critical(e.what());
    }

    logger.flush();
    return 0;
}
