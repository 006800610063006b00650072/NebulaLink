#pragma once
#include <string>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <boost/asio.hpp>
#include "Logger.h"

namespace asio = boost::asio;

class FileTransfer {
public:
    explicit FileTransfer(Logger& logger) : logger_(logger), io_context_(), socket_(io_context_) {}

    void sendFile(const std::string& filePath, const std::string& recipientIp, unsigned short port) {
        try {
            logger_.printAndLog("Connecting to " + recipientIp + ":" + std::to_string(port));
            asio::ip::tcp::endpoint endpoint(asio::ip::make_address(recipientIp), port);
            socket_.connect(endpoint);

            std::ifstream file(filePath, std::ios::binary);
            if (!file.is_open()) {
                logger_.printAndLog("Error opening file: " + filePath);
                return;
            }

            std::string fileName = std::filesystem::path(filePath).filename().string();
            send(socket_, fileName);

            std::ostringstream buffer;
            buffer << file.rdbuf();
            send(socket_, buffer.str());

            logger_.printAndLog("File sent successfully: " + filePath);
        } catch (const std::exception& e) {
            logger_.printAndLog(std::string("Error sending file: ") + e.what());
        }
    }

    void receiveFiles(unsigned short port) {
        try {
            std::string getDir = "GET";
            if (!std::filesystem::exists(getDir)) {
                std::filesystem::create_directory(getDir);
            }

            asio::ip::tcp::acceptor acceptor(io_context_, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port));
            logger_.printAndLog("Waiting for incoming file on port " + std::to_string(port));
            acceptor.accept(socket_);

            std::string fileName = receive(socket_);
            std::string fileContent = receive(socket_);

            std::ofstream file(getDir + "/" + fileName, std::ios::binary);
            file.write(fileContent.c_str(), fileContent.size());
            logger_.printAndLog("File received successfully: " + fileName);
        } catch (const std::exception& e) {
            logger_.printAndLog(std::string("Error receiving file: ") + e.what());
        }
    }

    void menu() {
        std::string command;
        while (true) {
            std::cout << "\nFile Transfer Menu:\n"
                      << "1. Send File\n"
                      << "2. Receive Files\n"
                      << "3. Back to Main Menu\n"
                      << "Choose an option: ";
            std::cin >> command;

            if (command == "1") {
                std::string filePath, recipientIp;
                unsigned short port;
                std::cout << "Enter file path: ";
                std::cin >> filePath;
                std::cout << "Enter recipient IP: ";
                std::cin >> recipientIp;
                std::cout << "Enter recipient port: ";
                std::cin >> port;
                sendFile(filePath, recipientIp, port);
            } else if (command == "2") {
                unsigned short port;
                std::cout << "Enter port to listen on: ";
                std::cin >> port;
                receiveFiles(port);
            } else if (command == "3") {
                break;
            } else {
                std::cout << "Invalid option. Try again.\n";
            }
        }
    }

private:
    void send(asio::ip::tcp::socket& socket, const std::string& data) {
        uint32_t size = data.size();
        asio::write(socket, asio::buffer(&size, sizeof(size)));
        asio::write(socket, asio::buffer(data));
    }

    std::string receive(asio::ip::tcp::socket& socket) {
        uint32_t size;
        asio::read(socket, asio::buffer(&size, sizeof(size)));

        std::vector<char> buffer(size);
        asio::read(socket, asio::buffer(buffer.data(), size));

        return std::string(buffer.begin(), buffer.end());
    }

    Logger& logger_;
    asio::io_context io_context_;
    asio::ip::tcp::socket socket_;
};
