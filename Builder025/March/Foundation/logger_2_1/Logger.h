#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
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

    LogError(std::string fileName = "app_log.log",
             bool showConsole = true,
             LogLevel minLevel = LogLevel::INFO);
    ~LogError();

    void log(LogLevel level, const std::string& message,
             const char* file = __FILE__,
             const char* func = __FUNCTION__,
             int line = __LINE__);
    
    void flush();

    // Convenience methods
    void info(const std::string& message, const char* file = __FILE__, 
              const char* func = __FUNCTION__, int line = __LINE__);
    void warning(const std::string& message, const char* file = __FILE__, 
                 const char* func = __FUNCTION__, int line = __LINE__);
    void error(const std::string& message, const char* file = __FILE__, 
               const char* func = __FUNCTION__, int line = __LINE__);
    void critical(const std::string& message, const char* file = __FILE__, 
                  const char* func = __FUNCTION__, int line = __LINE__);

    // Configuration methods
    void setConsoleOutput(bool enabled);
    void setLogFileName(const std::string& fileName);
    void setMinimumLevel(LogLevel level);
    void setMaxFileSize(size_t size);
    void setFormatter(std::function<std::string(LogLevel, const std::string&, 
                      const char*, const char*, int)> f);
    LogLevel getMinimumLevel() const;

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
    static constexpr size_t BUFFER_SIZE = 4096;
    size_t maxFileSize = 1024 * 1024;
    std::function<std::string(LogLevel, const std::string&, 
            const char*, const char*, int)> formatter;

    static constexpr const char* RESET_COLOR = "\033[0m";
    static constexpr const char* INFO_COLOR = "\033[32m";
    static constexpr const char* WARNING_COLOR = "\033[33m";
    static constexpr const char* ERROR_COLOR = "\033[31m";
    static constexpr const char* CRITICAL_COLOR = "\033[41m\033[37m";

    std::string getTimestamp();
    std::string levelToString(LogLevel level);
    std::string formatLocation(const char* file, const char* func, int line);
    const char* getColorCode(LogLevel level);
    std::string defaultFormatter(LogLevel level, const std::string& message, 
                               const char* file, const char* func, int line);
    void rotateLog();
    void logWorker();
};

#define LOG_IF_LEVEL(logger, level, msg) \
    do { if (static_cast<int>(level) >= static_cast<int>(logger.getMinimumLevel())) { \
        logger.log(level, msg, __FILE__, __FUNCTION__, __LINE__); \
    }} while(0)

#endif // LOGGER_H
