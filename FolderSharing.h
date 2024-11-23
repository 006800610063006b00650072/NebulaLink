#pragma once
#include <string>
#include <map>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>
#include <optional>
#include "Logger.h"
#include "PeerFinder.h"

namespace fs = std::filesystem;

class FolderSharing {
public:
    explicit FolderSharing(Logger& logger, PeerFinder& peerFinder)
        : logger_(logger), peerFinder_(peerFinder) {}

    // Объявить расшаренную папку для GUID
    void shareFolder(const std::string& guid, const std::string& folderPath) {
        if (!fs::exists(folderPath) || !fs::is_directory(folderPath)) {
            logger_.printAndLog("Invalid folder path: " + folderPath);
            return;
        }
        sharedFolders_[guid] = folderPath;
        logger_.printAndLog("Folder shared: " + folderPath + " for GUID: " + guid);
    }

    // Получить список доступных папок
    void listSharedFolders() {
        if (sharedFolders_.empty()) {
            logger_.printAndLog("No folders are currently shared.");
            return;
        }
        logger_.printAndLog("Shared Folders:");
        for (const auto& [guid, path] : sharedFolders_) {
            std::cout << "GUID: " << guid << " -> Path: " << path << "\n";
        }
    }

    // Просмотреть содержимое папки
    void viewFolderContents(const std::string& guid) {
        if (sharedFolders_.find(guid) == sharedFolders_.end()) {
            logger_.printAndLog("No folder shared for GUID: " + guid);
            return;
        }

        const std::string& folderPath = sharedFolders_[guid];
        logger_.printAndLog("Contents of folder shared for GUID: " + guid);
        for (const auto& entry : fs::directory_iterator(folderPath)) {
            std::cout << (entry.is_directory() ? "[DIR] " : "[FILE] ")
                      << entry.path().filename().string() << "\n";
        }
    }

    // Загрузить файл из расшаренной папки
    void downloadFile(const std::string& guid, const std::string& fileName, const std::string& destinationPath) {
        if (sharedFolders_.find(guid) == sharedFolders_.end()) {
            logger_.printAndLog("No folder shared for GUID: " + guid);
            return;
        }

        const std::string& folderPath = sharedFolders_[guid];
        const std::string sourceFilePath = folderPath + "/" + fileName;

        if (!fs::exists(sourceFilePath) || !fs::is_regular_file(sourceFilePath)) {
            logger_.printAndLog("File not found: " + fileName + " in folder for GUID: " + guid);
            return;
        }

        try {
            fs::copy(sourceFilePath, destinationPath, fs::copy_options::overwrite_existing);
            logger_.printAndLog("File downloaded: " + fileName + " to " + destinationPath);
        } catch (const fs::filesystem_error& e) {
            logger_.printAndLog("Error downloading file: " + std::string(e.what()));
        }
    }

    // Меню для работы с папками
    void menu() {
        std::string command;
        while (true) {
            std::cout << "\nFolder Sharing Menu:\n"
                      << "1. Share Folder\n"
                      << "2. List Shared Folders\n"
                      << "3. View Folder Contents\n"
                      << "4. Download File\n"
                      << "5. Back to Main Menu\n"
                      << "Choose an option: ";
            std::cin >> command;

            if (command == "1") {
                std::string guid, folderPath;
                std::cout << "Enter GUID: ";
                std::cin >> guid;
                std::cout << "Enter folder path: ";
                std::cin >> folderPath;
                shareFolder(guid, folderPath);
            } else if (command == "2") {
                listSharedFolders();
            } else if (command == "3") {
                std::string guid;
                std::cout << "Enter GUID: ";
                std::cin >> guid;
                viewFolderContents(guid);
            } else if (command == "4") {
                std::string guid, fileName, destinationPath;
                std::cout << "Enter GUID: ";
                std::cin >> guid;
                std::cout << "Enter file name: ";
                std::cin >> fileName;
                std::cout << "Enter destination path: ";
                std::cin >> destinationPath;
                downloadFile(guid, fileName, destinationPath);
            } else if (command == "5") {
                break;
            } else {
                std::cout << "Invalid option. Try again.\n";
            }
        }
    }

private:
    Logger& logger_;
    PeerFinder& peerFinder_;
    std::map<std::string, std::string> sharedFolders_; // GUID -> Folder Path
};

