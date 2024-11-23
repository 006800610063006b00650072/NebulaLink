#pragma once
#include <fstream>
#include <string>
#include <mutex>
#include <iostream>
#include <sstream> // Добавлено
#include <ctime>
#include <iomanip>

class Logger {
public:
    explicit Logger(const std::string& log_file = "log.txt") : log_file_(log_file) {}

    void log(const std::string& message) {
        std::lock_guard<std::mutex> lock(log_mutex_);
        std::ofstream log(log_file_, std::ios::app);
        if (log) {
            log << timestamp() << " " << message << std::endl;
        }
    }

    void printAndLog(const std::string& message) {
        std::cout << message << std::endl;
        log(message);
    }

private:
    std::string timestamp() {
        auto now = std::time(nullptr);
        std::ostringstream oss; // std::ostringstream теперь работает корректно
        oss << std::put_time(std::localtime(&now), "[%Y-%m-%d %H:%M:%S]");
        return oss.str();
    }

    std::string log_file_;
    std::mutex log_mutex_;
};
