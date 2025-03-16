#include "Logger.h"

LogError::LogError(std::string fileName, bool showConsole, LogLevel minLevel)
    : logFileName(fileName),
      consoleOutput(showConsole),
      minimumLevel(minLevel),
      running(true),
      formatter([this](LogLevel l, const std::string& m, const char* f, 
                      const char* fn, int ln) {
          return defaultFormatter(l, m, f, fn, ln);
      }) {
    logFile.open(fileName, std::ios::app);
    if (!logFile.is_open()) {
        std::cerr << "Failed to open log file: " << fileName << std::endl;
    }
    logThread = std::thread(&LogError::logWorker, this);
}

LogError::~LogError() {
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        running = false;
    }
    queueCondVar.notify_one();
    if (logThread.joinable()) logThread.join();
    if (logFile.is_open()) logFile.close();
}

std::string LogError::getTimestamp() {
    time_t now = time(nullptr);
    char buffer[80];
    struct tm* timeinfo = localtime(&now);
    strftime(buffer, 80, "%Y-%m-%d_%H-%M-%S", timeinfo);
    return std::string(buffer);
}

std::string LogError::levelToString(LogLevel level) {
    switch (level) {
        case LogLevel::INFO: return "INFO";
        case LogLevel::WARNING: return "WARNING";
        case LogLevel::ERROR: return "ERROR";
        case LogLevel::CRITICAL: return "CRITICAL";
        default: return "UNKNOWN";
    }
}

std::string LogError::formatLocation(const char* file, const char* func, int line) {
    return std::string(file) + ":" + std::to_string(line) + " [" + func + "]";
}

const char* LogError::getColorCode(LogLevel level) {
    switch (level) {
        case LogLevel::INFO: return INFO_COLOR;
        case LogLevel::WARNING: return WARNING_COLOR;
        case LogLevel::ERROR: return ERROR_COLOR;
        case LogLevel::CRITICAL: return CRITICAL_COLOR;
        default: return RESET_COLOR;
    }
}

std::string LogError::defaultFormatter(LogLevel level, const std::string& message, 
                                     const char* file, const char* func, int line) {
    return "[" + getTimestamp() + "] [" + formatLocation(file, func, line) + 
           "] [" + levelToString(level) + "] " + message;
}

void LogError::rotateLog() {
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

void LogError::logWorker() {
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
    std::lock_guard<std::mutex> fileLock(logMutex);
    if (logFile.is_open() && !buffer.empty()) {
        logFile << buffer;
        logFile.flush();
        buffer.clear();
    }
}

void LogError::log(LogLevel level, const std::string& message, 
                  const char* file, const char* func, int line) {
    if (static_cast<int>(level) < static_cast<int>(minimumLevel)) return;

    std::string fullMessage = formatter(level, message, file, func, line);

    {
        std::lock_guard<std::mutex> lock(queueMutex);
        logQueue.push(fullMessage);
    }
    queueCondVar.notify_one();

    if (consoleOutput) {
        std::ostream& outStream = (level == LogLevel::ERROR || level == LogLevel::CRITICAL) ? 
                                 std::cerr : std::cout;
        outStream << getColorCode(level) << fullMessage << RESET_COLOR << std::endl;
    }
}

void LogError::flush() {
    std::lock_guard<std::mutex> lock(logMutex);
    if (logFile.is_open() && !buffer.empty()) {
        logFile << buffer;
        logFile.flush();
        buffer.clear();
    }
}

void LogError::info(const std::string& message, const char* file, 
                   const char* func, int line) {
    log(LogLevel::INFO, message, file, func, line);
}

void LogError::warning(const std::string& message, const char* file, 
                      const char* func, int line) {
    log(LogLevel::WARNING, message, file, func, line);
}

void LogError::error(const std::string& message, const char* file, 
                    const char* func, int line) {
    log(LogLevel::ERROR, message, file, func, line);
}

void LogError::critical(const std::string& message, const char* file, 
                       const char* func, int line) {
    log(LogLevel::CRITICAL, message, file, func, line);
}

void LogError::setConsoleOutput(bool enabled) {
    std::lock_guard<std::mutex> lock(logMutex);
    consoleOutput = enabled;
}

void LogError::setLogFileName(const std::string& fileName) {
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

void LogError::setMinimumLevel(LogLevel level) {
    std::lock_guard<std::mutex> lock(logMutex);
    minimumLevel = level;
}

void LogError::setMaxFileSize(size_t size) {
    std::lock_guard<std::mutex> lock(logMutex);
    maxFileSize = size;
}

void LogError::setFormatter(std::function<std::string(LogLevel, const std::string&, 
                           const char*, const char*, int)> f) {
    std::lock_guard<std::mutex> lock(logMutex);
    formatter = f;
}

LogError::LogLevel LogError::getMinimumLevel() const {
    return minimumLevel;
}
