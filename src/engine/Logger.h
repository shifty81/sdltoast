#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>
#include <iostream>
#include <mutex>
#include <ctime>

/**
 * Simple logging utility that writes to both console and a log file.
 * Captures errors, warnings, and info messages during initialization and runtime.
 * Log file is written to "harvest_quest.log" by default.
 */
class Logger {
public:
    enum class Level {
        INFO,
        WARNING,
        ERROR
    };

    static Logger& Instance();

    /// Initialize the logger, opening the log file for writing.
    /// Call once at application startup.
    void Initialize(const std::string& logFilePath = "harvest_quest.log");

    /// Shut down the logger, flushing and closing the log file.
    void Shutdown();

    /// Log a message at the given severity level.
    void Log(Level level, const std::string& message);

    /// Convenience methods
    void Info(const std::string& message);
    void Warn(const std::string& message);
    void Error(const std::string& message);

    /// Returns true if any errors have been logged since initialization.
    bool HasErrors() const { return m_errorCount > 0; }

    /// Returns the number of errors logged.
    int GetErrorCount() const { return m_errorCount; }

    /// Returns the number of warnings logged.
    int GetWarningCount() const { return m_warningCount; }

private:
    Logger() = default;
    ~Logger();

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    std::string GetTimestamp() const;
    std::string LevelToString(Level level) const;

    std::ofstream m_logFile;
    std::mutex m_mutex;
    int m_errorCount = 0;
    int m_warningCount = 0;
    bool m_initialized = false;
};

#endif // LOGGER_H
