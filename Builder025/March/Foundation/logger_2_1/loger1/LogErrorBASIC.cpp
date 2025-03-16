#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <csignal>
#include <mutex>

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
        strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", timeinfo);
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

    // Get the color code based on log level
    const char* getColorCode(LogLevel level) {
        switch (level) {
            case LogLevel::INFO: return INFO_COLOR;
            case LogLevel::WARNING: return WARNING_COLOR;
            case LogLevel::ERROR: return ERROR_COLOR;
            case LogLevel::CRITICAL: return CRITICAL_COLOR;
            default: return RESET_COLOR;
        }
    }

public:
    LogError(std::string fileName = "app_log.log",
             bool showConsole = true,
             LogLevel minLevel = LogLevel::INFO)
        : logFileName(fileName),
          consoleOutput(showConsole),
          minimumLevel(minLevel) {
        std::lock_guard<std::mutex> lock(logMutex);
        logFile.open(fileName, std::ios::app);
        if (!logFile.is_open()) {
            std::cerr << "Failed to open log file: " + fileName << std::endl;
        }
    }

    ~LogError() {
        std::lock_guard<std::mutex> lock(logMutex);
        if (logFile.is_open()) {
            logFile.close();
        }
    }

    void log(LogLevel level, const std::string& message,
             const char* file = __FILE__,
             const char* func = __FUNCTION__,
             int line = __LINE__) {
        if (static_cast<int>(level) < static_cast<int>(minimumLevel)) {
            return;
        }

        std::lock_guard<std::mutex> lock(logMutex);

        std::string timestamp = getTimestamp();
        std::string fullMessage = "[" + timestamp + "] ";
        fullMessage += "[" + formatLocation(file, func, line) + "] ";
        std::string levelStr = "[" + levelToString(level) + "] ";
        fullMessage += levelStr + message;

        // Write to file (no color)
        if (logFile.is_open()) {
            logFile << fullMessage << std::endl;
            logFile.flush();
        }

        // Write to console with color
        if (consoleOutput) {
            std::ostream& outStream = (level == LogLevel::ERROR || level == LogLevel::CRITICAL) ? std::cerr : std::cout;
            outStream << getColorCode(level) << fullMessage << RESET_COLOR << std::endl;
        }
    }

    void flush() {
        std::lock_guard<std::mutex> lock(logMutex);
        if (logFile.is_open()) {
            logFile.flush();
        }
    }

    void info(const std::string& message,
              const char* file = __FILE__,
              const char* func = __FUNCTION__,
              int line = __LINE__) {
        log(LogLevel::INFO, message, file, func, line);
    }

    void warning(const std::string& message,
                 const char* file = __FILE__,
                 const char* func = __FUNCTION__,
                 int line = __LINE__) {
        log(LogLevel::WARNING, message, file, func, line);
    }

    void error(const std::string& message,
               const char* file = __FILE__,
               const char* func = __FUNCTION__,
               int line = __LINE__) {
        log(LogLevel::ERROR, message, file, func, line);
    }

    void critical(const std::string& message,
                  const char* file = __FILE__,
                  const char* func = __FUNCTION__,
                  int line = __LINE__) {
        log(LogLevel::CRITICAL, message, file, func, line);
    }

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
            std::cerr << "Failed to open log file: " + fileName << std::endl;
        }
    }

    void setMinimumLevel(LogLevel level) {
        std::lock_guard<std::mutex> lock(logMutex);
        minimumLevel = level;
    }
};

// Signal handler
void signalHandler(int signal) {
    LogError logger;
    logger.critical("Segmentation fault occurred");
    exit(signal);
}

// Main function
int main() {
    LogError logger("app_log.txt", true, LogError::LogLevel::INFO);
    signal(SIGSEGV, signalHandler);

    logger.info("Application started");

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

    logger.setMinimumLevel(LogError::LogLevel::WARNING);
    logger.info("This won't be logged due to minimum level");
    logger.warning("This will be logged");

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

    return 0;
}
