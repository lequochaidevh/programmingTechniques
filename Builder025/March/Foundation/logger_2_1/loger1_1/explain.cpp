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

// Defines a logging class to handle error and info logging with various features like async logging, log rotation, and colored console output
class LogError {
public:
    // Enumeration for log levels, defining severity of messages
    enum class LogLevel {
        INFO,      // Informational messages
        WARNING,   // Warnings that might indicate a problem
        ERROR,     // Errors that need attention
        CRITICAL   // Critical errors that might crash the program
    };

private:
    std::string logFileName;  // Name of the log file
    std::ofstream logFile;    // Output file stream for writing logs
    bool consoleOutput;       // Flag to enable/disable console output
    LogLevel minimumLevel;    // Minimum log level to filter messages
    std::mutex logMutex;      // Mutex for synchronizing file access
    std::mutex queueMutex;    // Mutex for synchronizing log queue access
    std::queue<std::string> logQueue;  // Queue for async log messages
    std::condition_variable queueCondVar;  // Condition variable to signal log thread
    std::thread logThread;    // Background thread for writing logs
    bool running;             // Flag to control log thread lifecycle
    std::string buffer;       // Buffer to batch log writes
    static constexpr size_t BUFFER_SIZE = 4096;  // Size of buffer before flushing (4KB)
    size_t maxFileSize = 1024 * 1024;  // Default max file size before rotation (1MB)
    std::function<std::string(LogLevel, const std::string&, const char*, const char*, int)> formatter;  // Customizable log message formatter

    // ANSI color codes for console output
    static constexpr const char* RESET_COLOR = "\033[0m";         // Resets color
    static constexpr const char* INFO_COLOR = "\033[32m";         // Green for INFO
    static constexpr const char* WARNING_COLOR = "\033[33m";      // Yellow for WARNING
    static constexpr const char* ERROR_COLOR = "\033[31m";        // Red for ERROR
    static constexpr const char* CRITICAL_COLOR = "\033[41m\033[37m";  // White on red for CRITICAL

    // Generates a timestamp string for log entries
    std::string getTimestamp() {
        time_t now = time(nullptr);  // Get current time
        char buffer[80];
        struct tm* timeinfo = localtime(&now);  // Convert to local time
        strftime(buffer, 80, "%Y-%m-%d_%H-%M-%S", timeinfo);  // Format as YYYY-MM-DD_HH-MM-SS
        return std::string(buffer);
    }

    // Converts log level enum to string for display
    std::string levelToString(LogLevel level) {
        switch (level) {
            case LogLevel::INFO: return "INFO";
            case LogLevel::WARNING: return "WARNING";
            case LogLevel::ERROR: return "ERROR";
            case LogLevel::CRITICAL: return "CRITICAL";
            default: return "UNKNOWN";  // Fallback for unexpected levels
        }
    }

    // Formats file, function, and line number into a location string
    std::string formatLocation(const char* file, const char* func, int line) {
        return std::string(file) + ":" + std::to_string(line) + " [" + func + "]";
    }

    // Returns ANSI color code based on log level
    const char* getColorCode(LogLevel level) {
        switch (level) {
            case LogLevel::INFO: return INFO_COLOR;
            case LogLevel::WARNING: return WARNING_COLOR;
            case LogLevel::ERROR: return ERROR_COLOR;
            case LogLevel::CRITICAL: return CRITICAL_COLOR;
            default: return RESET_COLOR;  // Default to no color
        }
    }

    // Default formatter for log messages, combining timestamp, location, level, and message
    std::string defaultFormatter(LogLevel level, const std::string& message, const char* file, const char* func, int line) {
        return "[" + getTimestamp() + "] [" + formatLocation(file, func, line) + "] [" + levelToString(level) + "] " + message;
    }

    // Rotates log file when it exceeds maxFileSize by renaming it with a timestamp
    void rotateLog() {
        if (logFile.is_open()) {
            logFile.close();  // Close current file
            std::string newName = logFileName + "." + getTimestamp() + ".bak";  // New name with timestamp
            std::rename(logFileName.c_str(), newName.c_str());  // Rename old file
            logFile.open(logFileName, std::ios::app);  // Reopen original file for appending
            if (!logFile.is_open()) {
                std::cerr << "Failed to reopen log file after rotation: " << logFileName << std::endl;
            }
        }
    }

    // Background thread function to process log queue and write to file
    void logWorker() {
        while (running || !logQueue.empty()) {  // Run while active or queue has items
            std::unique_lock<std::mutex> lock(queueMutex);
            if (logQueue.empty()) {
                queueCondVar.wait(lock);  // Wait for new log entries
                if (!running && logQueue.empty()) break;  // Exit if shutting down and queue is empty
            }
            std::string message = std::move(logQueue.front());  // Get next message
            logQueue.pop();  // Remove it from queue
            lock.unlock();

            std::lock_guard<std::mutex> fileLock(logMutex);
            if (logFile.is_open()) {
                // Check if adding message exceeds file size limit
                if (logFile.tellp().operator+(buffer.size() + message.size()) >= maxFileSize) {
                    if (!buffer.empty()) {
                        logFile << buffer;  // Write current buffer
                        buffer.clear();
                    }
                    rotateLog();  // Rotate file if size limit reached
                }
                buffer += message + "\n";  // Add message to buffer
                // Flush buffer if full or critical message detected
                if (buffer.size() >= BUFFER_SIZE || message.find("[CRITICAL]") != std::string::npos) {
                    logFile << buffer;
                    logFile.flush();
                    buffer.clear();
                }
            }
        }
        // Final flush on shutdown
        std::lock_guard<std::mutex> fileLock(logMutex);
        if (logFile.is_open() && !buffer.empty()) {
            logFile << buffer;
            logFile.flush();
            buffer.clear();
        }
    }

public:
    // Constructor: initializes logger with file name, console output, and minimum log level
    LogError(std::string fileName = "app_log.log",
             bool showConsole = true,
             LogLevel minLevel = LogLevel::INFO)
        : logFileName(fileName),
          consoleOutput(showConsole),
          minimumLevel(minLevel),
          running(true),
          formatter([this](LogLevel l, const std::string& m, const char* f, const char* fn, int ln) {
              return defaultFormatter(l, m, f, fn, ln);  // Set default formatter
          }) {
        logFile.open(fileName, std::ios::app);  // Open log file in append mode
        if (!logFile.is_open()) {
            std::cerr << "Failed to open log file: " << fileName << std::endl;
        }
        logThread = std::thread(&LogError::logWorker, this);  // Start log worker thread
    }

    // Destructor: cleans up resources
    ~LogError() {
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            running = false;  // Signal thread to stop
        }
        queueCondVar.notify_one();  // Wake up log thread
        if (logThread.joinable()) logThread.join();  // Wait for thread to finish
        if (logFile.is_open()) logFile.close();  // Close file
    }

    // Core logging function: adds message to queue and optionally prints to console
    void log(LogLevel level, const std::string& message,
             const char* file = __FILE__,
             const char* func = __FUNCTION__,
             int line = __LINE__) {
        if (static_cast<int>(level) < static_cast<int>(minimumLevel)) return;  // Filter by level

        std::string fullMessage = formatter(level, message, file, func, line);  // Format message

        {
            std::lock_guard<std::mutex> lock(queueMutex);
            logQueue.push(fullMessage);  // Add to queue
        }
        queueCondVar.notify_one();  // Notify worker thread

        if (consoleOutput) {  // Print to console with color if enabled
            std::ostream& outStream = (level == LogLevel::ERROR || level == LogLevel::CRITICAL) ? std::cerr : std::cout;
            outStream << getColorCode(level) << fullMessage << RESET_COLOR << std::endl;
        }
    }

    // Forces buffer to be written to file
    void flush() {
        std::lock_guard<std::mutex> lock(logMutex);
        if (logFile.is_open() && !buffer.empty()) {
            logFile << buffer;
            logFile.flush();
            buffer.clear();
        }
    }

    // Convenience methods for specific log levels
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
        consoleOutput = enabled;  // Toggle console output
    }

    void setLogFileName(const std::string& fileName) {
        std::lock_guard<std::mutex> lock(logMutex);
        if (logFile.is_open()) logFile.close();  // Close existing file
        logFileName = fileName;
        logFile.open(fileName, std::ios::app);  // Open new file
        if (!logFile.is_open()) {
            std::cerr << "Failed to open log file: " << fileName << std::endl;
        }
    }

    void setMinimumLevel(LogLevel level) {
        std::lock_guard<std::mutex> lock(logMutex);
        minimumLevel = level;  // Set minimum log level
    }

    void setMaxFileSize(size_t size) {
        std::lock_guard<std::mutex> lock(logMutex);
        maxFileSize = size;  // Set max file size for rotation
    }

    void setFormatter(std::function<std::string(LogLevel, const std::string&, const char*, const char*, int)> f) {
        std::lock_guard<std::mutex> lock(logMutex);
        formatter = f;  // Set custom formatter
    }

    LogLevel getMinimumLevel() const { return minimumLevel; }  // Getter for minimum level
};

// Macro for runtime log level filtering (original version without constexpr)
#define LOG_IF_LEVEL(logger, level, msg) \
    do { if (static_cast<int>(level) >= static_cast<int>(logger.getMinimumLevel())) { \
        logger.log(level, msg, __FILE__, __FUNCTION__, __LINE__); \
    }} while(0)

// Signal handler for segmentation faults
void signalHandler(int signal) {
    LogError logger;  // Create temporary logger instance
    logger.critical("Segmentation fault occurred");  // Log critical message
    exit(signal);  // Exit with signal code
}

// Main function demonstrating logger features
int main() {
    LogError logger("app_log.log", true, LogError::LogLevel::INFO);  // Initialize logger
    signal(SIGSEGV, signalHandler);  // Register signal handler

    // Set a custom formatter with a simpler format
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

    logger.info("Application started with custom formatter");  // Log startup message

    // Demonstrate log rotation with small file size
    logger.setMaxFileSize(1024);  // Set to 1KB for demo
    for (int i = 0; i < 100; ++i) {
        logger.info("Log entry " + std::to_string(i));  // Generate multiple log entries
    }

    // Demonstrate runtime log filtering with macro
    logger.setMinimumLevel(LogError::LogLevel::WARNING);  // Raise minimum level
    LOG_IF_LEVEL(logger, LogError::LogLevel::INFO, "This won't be logged due to runtime check");
    LOG_IF_LEVEL(logger, LogError::LogLevel::WARNING, "This will be logged");

    // Demonstrate async logging and exception handling
    try {
        int divisor = 0;
        if (divisor == 0) {
            logger.warning("Potential division by zero detected");  // Warn before exception
            throw std::runtime_error("Division by zero attempted");  // Throw exception
        }
        int result = 10 / divisor;  // This won't execute
    }
    catch (const std::exception& e) {
        logger.error(e.what());  // Log exception message
    }

    // Demonstrate critical log with immediate flush
    try {
        int* ptr = nullptr;
        if (ptr == nullptr) {
            logger.warning("Null pointer detected");  // Warn before exception
            throw std::runtime_error("Null pointer access attempted");  // Throw exception
        }
        *ptr = 42;  // This causes segmentation fault if reached
    }
    catch (const std::exception& e) {
        logger.critical(e.what());  // Log critical message with immediate flush
    }

    logger.flush();  // Ensure all logs are written before exit
    return 0;
}
