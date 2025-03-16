#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <csignal>
#include <mutex>
#include <queue>
#include <thread>
#include <condition_variable>
#include <functional>

class LogError {
public:
    enum class LogLevel {
        INFO,
        WARNING,
        ERROR,
        CRITICAL
    };

private:
    std::string logFileName;
    std::ofstream logFile;
    bool consoleOutput;
    LogLevel minimumLevel;
    std::mutex logMutex;
    std::mutex queueMutex;
    std::queue<std::string> logQueue;
    std::condition_variable queueCondVar;
    std::thread logThread;
    bool running;
    std::string buffer;
    static constexpr size_t BUFFER_SIZE = 4096; // Buffer size for writes
    size_t maxFileSize = 1024 * 1024; // 1MB default for log rotation
    std::function<std::string(LogLevel, const std::string&, const char*, const char*, int)> formatter;

    // ANSI color codes
    static constexpr const char* RESET_COLOR = "\033[0m";
    static constexpr const char* INFO_COLOR = "\033[32m";    // Green
    static constexpr const char* WARNING_COLOR = "\033[33m"; // Yellow
    static constexpr const char* ERROR_COLOR = "\033[31m";   // Red
    static constexpr const char* CRITICAL_COLOR = "\033[41m\033[37m"; // White text on Red background

    std::string getTimestamp() {
        time_t now = time(nullptr);
        char buffer[80];
        struct tm* timeinfo = localtime(&now);
        strftime(buffer, 80, "%Y-%m-%d_%H-%M-%S", timeinfo); // Adjusted for log rotation naming
        return std::string(buffer);
    }

    std::string levelToString(LogLevel level) {
        switch (level) {
            case LogLevel::INFO: return "INFO";
            case LogLevel::WARNING: return "WARNING";
            case LogLevel::ERROR: return "ERROR";
            case LogLevel::CRITICAL: return "CRITICAL";
            default: return "UNKNOWN";
        }
    }

    std::string formatLocation(const char* file, const char* func, int line) {
        return std::string(file) + ":" + std::to_string(line) + " [" + func + "]";
    }

    const char* getColorCode(LogLevel level) {
        switch (level) {
            case LogLevel::INFO: return INFO_COLOR;
            case LogLevel::WARNING: return WARNING_COLOR;
            case LogLevel::ERROR: return ERROR_COLOR;
            case LogLevel::CRITICAL: return CRITICAL_COLOR;
            default: return RESET_COLOR;
        }
    }

    std::string defaultFormatter(LogLevel level, const std::string& message, const char* file, const char* func, int line) {
        return "[" + getTimestamp() + "] [" + formatLocation(file, func, line) + "] [" + levelToString(level) + "] " + message;
    }

    void rotateLog() {
        if (logFile.is_open()) {
            logFile.close();
            std::string newName = logFileName + "." + getTimestamp() + ".bak";
            std::rename(logFileName.c_str(), newName.c_str());
            logFile.open(logFileName, std::ios::app);
            if (!logFile.is_open()) {
                std::cerr << "Failed to reopen log file after rotation: " << logFileName << std::endl;
            }
        }
    }

    void logWorker() {
        while (running || !logQueue.empty()) {
            std::unique_lock<std::mutex> lock(queueMutex);
            if (logQueue.empty()) {
                queueCondVar.wait(lock);
                if (!running && logQueue.empty()) break;
            }
            std::string message = std::move(logQueue.front());
            logQueue.pop();
            lock.unlock();

            std::lock_guard<std::mutex> fileLock(logMutex);
            if (logFile.is_open()) {
                if (logFile.tellp().operator+(buffer.size() + message.size()) >= maxFileSize) {
                    if (!buffer.empty()) {
                        logFile << buffer;
                        buffer.clear();
                    }
                    rotateLog();
                }
                buffer += message + "\n";
                if (buffer.size() >= BUFFER_SIZE || message.find("[CRITICAL]") != std::string::npos) {
                    logFile << buffer;
                    logFile.flush();
                    buffer.clear();
                }
            }
        }
        // Flush remaining buffer on shutdown
        std::lock_guard<std::mutex> fileLock(logMutex);
        if (logFile.is_open() && !buffer.empty()) {
            logFile << buffer;
            logFile.flush();
            buffer.clear();
        }
    }

public:
    LogError(std::string fileName = "app_log.log",
             bool showConsole = true,
             LogLevel minLevel = LogLevel::INFO)
        : logFileName(fileName),
          consoleOutput(showConsole),
          minimumLevel(minLevel),
          running(true),
          formatter([this](LogLevel l, const std::string& m, const char* f, const char* fn, int ln) {
              return defaultFormatter(l, m, f, fn, ln);
          }) {
        logFile.open(fileName, std::ios::app);
        if (!logFile.is_open()) {
            std::cerr << "Failed to open log file: " << fileName << std::endl;
        }
        logThread = std::thread(&LogError::logWorker, this);
    }

    ~LogError() {
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            running = false;
        }
        queueCondVar.notify_one();
        if (logThread.joinable()) logThread.join();
        if (logFile.is_open()) logFile.close();
    }

    void log(LogLevel level, const std::string& message,
             const char* file = __FILE__,
             const char* func = __FUNCTION__,
             int line = __LINE__) {
        if (static_cast<int>(level) < static_cast<int>(minimumLevel)) return;

        std::string fullMessage = formatter(level, message, file, func, line);

        {
            std::lock_guard<std::mutex> lock(queueMutex);
            logQueue.push(fullMessage);
        }
        queueCondVar.notify_one();

        if (consoleOutput) {
            std::ostream& outStream = (level == LogLevel::ERROR || level == LogLevel::CRITICAL) ? std::cerr : std::cout;
            outStream << getColorCode(level) << fullMessage << RESET_COLOR << std::endl;
        }
    }

    void flush() {
        std::lock_guard<std::mutex> lock(logMutex);
        if (logFile.is_open() && !buffer.empty()) {
            logFile << buffer;
            logFile.flush();
            buffer.clear();
        }
    }

    // Convenience methods
    void info(const std::string& message, const char* file = __FILE__, const char* func = __FUNCTION__, int line = __LINE__) {
        log(LogLevel::INFO, message, file, func, line);
    }

    void warning(const std::string& message, const char* file = __FILE__, const char* func = __FUNCTION__, int line = __LINE__) {
        log(LogLevel::WARNING, message, file, func, line);
    }

    void error(const std::string& message, const char* file = __FILE__, const char* func = __FUNCTION__, int line = __LINE__) {
        log(LogLevel::ERROR, message, file, func, line);
    }

    void critical(const std::string& message, const char* file = __FILE__, const char* func = __FUNCTION__, int line = __LINE__) {
        log(LogLevel::CRITICAL, message, file, func, line);
    }

    // Configuration methods
    void setConsoleOutput(bool enabled) {
        std::lock_guard<std::mutex> lock(logMutex);
        consoleOutput = enabled;
    }

    void setLogFileName(const std::string& fileName) {
        std::lock_guard<std::mutex> lock(logMutex);
        if (logFile.is_open()) {
            logFile.close();
        }
        logFileName = fileName;
        logFile.open(fileName, std::ios::app);
        if (!logFile.is_open()) {
            std::cerr << "Failed to open log file: " << fileName << std::endl;
        }
    }

    void setMinimumLevel(LogLevel level) {
        std::lock_guard<std::mutex> lock(logMutex);
        minimumLevel = level;
    }

    void setMaxFileSize(size_t size) {
        std::lock_guard<std::mutex> lock(logMutex);
        maxFileSize = size;
    }

    void setFormatter(std::function<std::string(LogLevel, const std::string&, const char*, const char*, int)> f) {
        std::lock_guard<std::mutex> lock(logMutex);
        formatter = f;
    }

    LogLevel getMinimumLevel() const { return minimumLevel; }
};

// Macro for compile-time log level filtering
    //g++ -std=c++17 LogError.cpp
//#define LOG_IF_LEVEL(logger, level, msg) \
//    do { if constexpr (static_cast<int>(level) >= static_cast<int>(logger.getMinimumLevel())) { \
//        logger.log(level, msg); \
//    }} while (0)

#define LOG_IF_LEVEL(logger, level, msg) \
    do { if (static_cast<int>(level) >= static_cast<int>(logger.getMinimumLevel())) { \
        logger.log(level, msg, __FILE__, __FUNCTION__, __LINE__); \
    }} while(0)
// Signal handler
void signalHandler(int signal) {
    LogError logger;
    logger.critical("Segmentation fault occurred");
    exit(signal);
}

// Main function demonstrating new features
int main() {
    LogError logger("app_log.log", true, LogError::LogLevel::INFO);
    signal(SIGSEGV, signalHandler);

    // Set a custom formatter
    logger.setFormatter([](LogError::LogLevel level, const std::string& msg, const char* file, const char* func, int line) -> std::string {
            std::string levelStr;
            switch (level) {
                case LogError::LogLevel::INFO: levelStr = "INFO"; break;
                case LogError::LogLevel::WARNING: levelStr = "WARNING"; break;
                case LogError::LogLevel::ERROR: levelStr = "ERROR"; break;
                default: levelStr = "UNKNOWN"; break;
            }
            return "[" + levelStr + "] " + std::string(file) + ":" + std::string(func) + ":" + std::to_string(line) + " - " + msg;
        });

    logger.info("Application started with custom formatter");

    // Demonstrate log rotation (set small size for testing)
    logger.setMaxFileSize(1024); // 1KB for demo purposes
    for (int i = 0; i < 100; ++i) {
        logger.info("Log entry " + std::to_string(i));
    }

    // Demonstrate compile-time filtering
    logger.setMinimumLevel(LogError::LogLevel::WARNING);
    LOG_IF_LEVEL(logger, LogError::LogLevel::INFO, "This won't be logged due to compile-time check");
    LOG_IF_LEVEL(logger, LogError::LogLevel::WARNING, "This will be logged");

    // Demonstrate async logging and buffering
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

    // Demonstrate critical log triggering immediate flush
    try {
        int* ptr = nullptr;
        if (ptr == nullptr) {
            logger.warning("Null pointer detected");
            throw std::runtime_error("Null pointer access attempted");
        }
        *ptr = 42;
    }
    catch (const std::exception& e) {
        logger.critical(e.what());
    }

    logger.flush(); // Ensure all buffered logs are written before exit
    return 0;
}
