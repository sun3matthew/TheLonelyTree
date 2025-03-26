#include <engine/logger.h>
#include <iostream>

// Static members
std::ofstream Logger::logFile;
std::queue<std::string> Logger::logQueue;
std::mutex Logger::mutex;
std::condition_variable Logger::condVar;
std::atomic<bool> Logger::running = false;
std::thread Logger::worker;

void Logger::init(const std::string& filename) {
    if (running) return; // already running

    logFile.open(filename, std::ios::out | std::ios::app);
    running = true;
    worker = std::thread(Logger::run);
}

void Logger::shutdown() {
    if (!running) return;

    running = false;
    condVar.notify_all();
    if (worker.joinable())
        worker.join();

    std::lock_guard<std::mutex> lock(mutex);
    while (!logQueue.empty()) {
        logFile << logQueue.front() << std::endl;
        logQueue.pop();
    }

    logFile.flush();
    logFile.close();
}

void Logger::log(const std::string& message) {
    {
        std::lock_guard<std::mutex> lock(mutex);
        logQueue.push(message);
    }
    condVar.notify_one();

    if (logToConsole) {
        std::cout << message << std::endl;
    }
}

void Logger::log_now(const std::string& message) {
    std::lock_guard<std::mutex> lock(mutex);
    logFile << message << std::endl;
    logFile.flush();

    if (logToConsole) {
        std::cout << message << std::endl;
    }
}

void Logger::flush() {
    std::lock_guard<std::mutex> lock(mutex);
    logFile.flush();
}

void Logger::run() {
    while (running) {
        std::unique_lock<std::mutex> lock(mutex);
        condVar.wait(lock, [] {
            return !logQueue.empty() || !running;
        });

        while (!logQueue.empty()) {
            logFile << logQueue.front() << std::endl;
            logQueue.pop();
        }

        logFile.flush();
    }
}
