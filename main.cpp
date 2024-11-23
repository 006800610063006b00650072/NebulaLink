#include <iostream>
#include <string>
#include <filesystem>
#include "Logger.h"
#include "AddressBook.h"
#include "Chats.h"
#include "FolderSharing.h"
#include "FileTransfer.h"
#include "RemoteControl.h"
#include "PeerFinder.h"

// Функция для отображения ASCII-Art
void printWelcomeMessage() {
    std::cout << R"(

███╗   ██╗███████╗██████╗ ██╗   ██╗██╗      █████╗     ██╗     ██╗███╗   ██╗██╗  ██╗
████╗  ██║██╔════╝██╔══██╗██║   ██║██║     ██╔══██╗    ██║     ██║████╗  ██║██║ ██╔╝
██╔██╗ ██║█████╗  ██████╔╝██║   ██║██║     ███████║    ██║     ██║██╔██╗ ██║█████╔╝ 
██║╚██╗██║██╔══╝  ██╔══██╗██║   ██║██║     ██╔══██║    ██║     ██║██║╚██╗██║██╔═██╗ 
██║ ╚████║███████╗██████╔╝╚██████╔╝███████╗██║  ██║    ███████╗██║██║ ╚████║██║  ██╗
╚═╝  ╚═══╝╚══════╝╚═════╝  ╚═════╝ ╚══════╝╚═╝  ╚═╝    ╚══════╝╚═╝╚═╝  ╚═══╝╚═╝  ╚═╝
                                                                                    

        Welcome to NebulaLink!
    Your Decentralized Communication Hub
    )" << std::endl;
}

// Меню работы с PeerFinder
void peerFinderMenu(PeerFinder& peerFinder) {
    std::string command;
    while (true) {
        std::cout << "\nPeerFinder Menu:\n"
                  << "1. Announce GUID\n"
                  << "2. Find Peer by GUID\n"
                  << "3. Show All Known Peers\n"
                  << "4. Back to Main Menu\n"
                  << "Choose an option: ";
        std::cin >> command;

        if (command == "1") {
            std::string guid, ip;
            unsigned short port;
            std::cout << "Enter GUID: ";
            std::cin >> guid;
            std::cout << "Enter IP: ";
            std::cin >> ip;
            std::cout << "Enter Port: ";
            std::cin >> port;
            peerFinder.announceGUID(guid, ip, port);
        } else if (command == "2") {
            std::string guid;
            std::cout << "Enter GUID to search for: ";
            std::cin >> guid;
            auto peerInfo = peerFinder.findPeer(guid);
            if (peerInfo) {
                auto [ip, port] = *peerInfo;
                std::cout << "Peer found! IP: " << ip << ", Port: " << port << "\n";
            } else {
                std::cout << "Peer not found for GUID: " << guid << "\n";
            }
        } else if (command == "3") {
            std::cout << "Known Peers:\n";
            auto peers = peerFinder.getKnownPeers();
            for (const auto& [guid, info] : peers) {
                auto [ip, port] = info;
                std::cout << "GUID: " << guid << " -> IP: " << ip << ", Port: " << port << "\n";
            }
        } else if (command == "4") {
            break;
        } else {
            std::cout << "Invalid option. Try again.\n";
        }
    }
}

// Основное меню
void mainMenu(AddressBook& addressBook, Chats& chats, FolderSharing& folderSharing, FileTransfer& fileTransfer, RemoteControl& remoteControl, PeerFinder& peerFinder) {
    std::string command;
    while (true) {
        std::cout << "\nMain Menu:\n"
                  << "1. Address Book\n"
                  << "2. Chats\n"
                  << "3. Folder Sharing\n"
                  << "4. File Transfer\n"
                  << "5. Remote Control\n"
                  << "6. Peer Finder\n"
                  << "7. Exit\n"
                  << "Choose an option: ";
        std::cin >> command;

        if (command == "1") {
            addressBook.menu();
        } else if (command == "2") {
            chats.menu();
        } else if (command == "3") {
            folderSharing.menu();
        } else if (command == "4") {
            fileTransfer.menu();
        } else if (command == "5") {
            remoteControl.menu();
        } else if (command == "6") {
            peerFinderMenu(peerFinder);
        } else if (command == "7") {
            break;
        } else {
            std::cout << "Invalid option. Try again.\n";
        }
    }
}

int main() {
    Logger logger("NebulaLink.log");
    printWelcomeMessage();

    // Инициализация PeerFinder
    PeerFinder peerFinder(logger);
    peerFinder.startAutoDiscovery(); // Запуск автоматического поиска пиров

    // Инициализация модулей
    AddressBook addressBook(logger);
    Chats chats(logger, peerFinder); // Обновлено
    FolderSharing folderSharing(logger, peerFinder); // Обновлено
    FileTransfer fileTransfer(logger);
    RemoteControl remoteControl(logger);

    mainMenu(addressBook, chats, folderSharing, fileTransfer, remoteControl, peerFinder);

    return 0;
}
