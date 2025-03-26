#pragma once

#include <string>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <fstream>

class Logger {
public:
    static void init(const std::string& filename);
    static void shutdown();

    static void log(const std::string& message);
    static void log_now(const std::string& message);
    static void flush();

private:
    static void run();

    Logger() = delete;

    static std::ofstream logFile;
    static std::queue<std::string> logQueue;
    static std::mutex mutex;
    static std::condition_variable condVar;
    static std::atomic<bool> running;
    static std::thread worker;

#ifdef DEBUG_PRINT
    static constexpr bool logToConsole = true;
#else
    // static constexpr bool logToConsole = false;
    static constexpr bool logToConsole = true;
#endif
};
