#pragma once
#include <string>
#include <iostream>
#include <thread>
#include <mutex>
#include <optional>
#include <vector>
#include <boost/asio.hpp>
#include <libtorrent/session.hpp>
#include <libtorrent/alert_types.hpp>
#include <libtorrent/sha1_hash.hpp>
#include <libtorrent/bdecode.hpp>
#include "Logger.h"
#include "PeerFinder.h"

namespace asio = boost::asio;
using tcp = asio::ip::tcp;
namespace lt = libtorrent;

class Chats {
public:
    explicit Chats(Logger& logger, PeerFinder& peerFinder)
        : logger_(logger), peerFinder_(peerFinder), io_context_(), acceptor_(io_context_) {}

    // Запуск сервера для приёма сообщений
    void startServer(unsigned short port) {
        try {
            tcp::endpoint endpoint(tcp::v4(), port);
            acceptor_.open(endpoint.protocol());
            acceptor_.set_option(asio::socket_base::reuse_address(true));
            acceptor_.bind(endpoint);
            acceptor_.listen();

            logger_.printAndLog("Chat server started on port " + std::to_string(port));

            std::thread([this]() {
                while (true) {
                    tcp::socket socket(io_context_);
                    acceptor_.accept(socket);

                    std::thread(&Chats::handleIncomingMessage, this, std::move(socket)).detach();
                }
            }).detach();
        } catch (const std::exception& e) {
            logger_.printAndLog(std::string("Error starting server: ") + e.what());
        }
    }

    // Отправка сообщения по UUID
    void sendMessageByUUID(const std::string& recipientUUID, const std::string& message) {
        auto peerInfo = peerFinder_.findPeer(recipientUUID);

        if (!peerInfo) {
            logger_.printAndLog("No peer found for UUID: " + recipientUUID);
            return;
        }

        auto [ip, port] = *peerInfo;
        logger_.printAndLog("Found peer: " + ip + ":" + std::to_string(port));

        try {
            tcp::socket socket(io_context_);
            tcp::endpoint endpoint(asio::ip::make_address(ip), port);

            logger_.printAndLog("Connecting to " + ip + ":" + std::to_string(port));
            socket.connect(endpoint);

            send(socket, message);
            logger_.printAndLog("Message sent to UUID: " + recipientUUID);
        } catch (const std::exception& e) {
            logger_.printAndLog(std::string("Error sending message to UUID: ") + e.what());
        }
    }

    // Меню для работы с чатами
    void menu() {
        std::string command;
        while (true) {
            std::cout << "\nChats Menu:\n"
                      << "1. Send Message by UUID\n"
                      << "2. Start Server (for incoming messages)\n"
                      << "3. Back to Main Menu\n"
                      << "Choose an option: ";
            std::cin >> command;

            if (command == "1") {
                std::string recipientUUID, message;
                std::cout << "Enter recipient UUID: ";
                std::cin >> recipientUUID;
                std::cout << "Enter message: ";
                std::cin.ignore();
                std::getline(std::cin, message);
                sendMessageByUUID(recipientUUID, message);
            } else if (command == "2") {
                unsigned short port;
                std::cout << "Enter port to listen on: ";
                std::cin >> port;
                startServer(port);
            } else if (command == "3") {
                break;
            } else {
                std::cout << "Invalid option. Try again.\n";
            }
        }
    }

private:
    Logger& logger_;
    PeerFinder& peerFinder_;
    asio::io_context io_context_;
    tcp::acceptor acceptor_;

    // Отправка данных через сокет
    void send(tcp::socket& socket, const std::string& message) {
        uint32_t size = message.size();
        asio::write(socket, asio::buffer(&size, sizeof(size)));
        asio::write(socket, asio::buffer(message));
    }

    // Приём данных через сокет
    std::string receive(tcp::socket& socket) {
        uint32_t size;
        asio::read(socket, asio::buffer(&size, sizeof(size)));

        std::vector<char> buffer(size);
        asio::read(socket, asio::buffer(buffer.data(), size));

        return std::string(buffer.begin(), buffer.end());
    }

    // Обработка входящих сообщений
    void handleIncomingMessage(tcp::socket socket) {
        try {
            std::string message = receive(socket);
            logger_.printAndLog("Message received: " + message);
        } catch (const std::exception& e) {
            logger_.printAndLog(std::string("Error handling incoming message: ") + e.what());
        }
    }
};

