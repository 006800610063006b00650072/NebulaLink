#pragma once
#include <string>
#include <iostream>
#include <thread>
#include <array>
#include <stdexcept>
#include "Logger.h"

class RemoteControl {
public:
    explicit RemoteControl(Logger& logger) : logger_(logger) {}

    // Обработка команд
    void executeCommand(const std::string& command) {
        try {
            logger_.printAndLog("Executing command: " + command);
            std::string result = runCommand(command);
            logger_.printAndLog("Command output:\n" + result);
        } catch (const std::exception& e) {
            logger_.printAndLog("Error executing command: " + std::string(e.what()));
        }
    }

    // Запуск интерактивного shell
    void startShell() {
        logger_.printAndLog("Starting shell...");
#ifdef _WIN32
        system("cmd.exe");
#else
        system("/bin/bash");
#endif
        logger_.printAndLog("Shell session ended.");
    }

    // Меню для удалённого управления
    void menu() {
        std::string command;
        while (true) {
            std::cout << "\nRemote Control Menu:\n"
                      << "1. Execute Command\n"
                      << "2. Start Shell\n"
                      << "3. Back to Main Menu\n"
                      << "Choose an option: ";
            std::cin >> command;

            if (command == "1") {
                std::cout << "Enter command: ";
                std::cin.ignore();
                std::getline(std::cin, command);
                executeCommand(command);
            } else if (command == "2") {
                startShell();
            } else if (command == "3") {
                break;
            } else {
                std::cout << "Invalid option. Try again.\n";
            }
        }
    }

private:
    Logger& logger_;

    // Выполнение команды в терминале
    std::string runCommand(const std::string& command) {
        std::array<char, 128> buffer;
        std::string result;
#ifdef _WIN32
        std::shared_ptr<FILE> pipe(_popen(command.c_str(), "r"), _pclose);
#else
        std::shared_ptr<FILE> pipe(popen(command.c_str(), "r"), pclose);
#endif
        if (!pipe) throw std::runtime_error("Failed to open pipe.");
        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
            result += buffer.data();
        }
        return result;
    }
};

