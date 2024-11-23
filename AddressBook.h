#pragma once
#include <string>
#include <map>
#include <vector>
#include <optional>
#include "Logger.h"

class AddressBook {
public:
    explicit AddressBook(Logger& logger) : logger_(logger) {}

    void addContact(const std::string& name, const std::string& guid) {
        contacts_[guid] = name;
        logger_.printAndLog("Added contact: " + name + " with GUID: " + guid);
    }

    void addGroup(const std::string& groupName, const std::vector<std::string>& guids) {
        groups_[groupName] = guids;
        logger_.printAndLog("Added group: " + groupName);
    }

    std::optional<std::string> getContactName(const std::string& guid) {
        return contacts_.count(guid) ? std::optional(contacts_[guid]) : std::nullopt;
    }

    void menu() {
        std::string command;
        while (true) {
            std::cout << "\nAddress Book Menu:\n"
                      << "1. Add Contact\n"
                      << "2. Add Group\n"
                      << "3. View Contact\n"
                      << "4. Back to Main Menu\n"
                      << "Choose an option: ";
            std::cin >> command;

            if (command == "1") {
                std::string name, guid;
                std::cout << "Enter contact name: ";
                std::cin >> name;
                std::cout << "Enter GUID: ";
                std::cin >> guid;
                addContact(name, guid);
            } else if (command == "2") {
                std::string groupName;
                int numContacts;
                std::vector<std::string> guids;

                std::cout << "Enter group name: ";
                std::cin >> groupName;
                std::cout << "Enter number of contacts: ";
                std::cin >> numContacts;
                for (int i = 0; i < numContacts; ++i) {
                    std::string guid;
                    std::cout << "Enter GUID " << i + 1 << ": ";
                    std::cin >> guid;
                    guids.push_back(guid);
                }
                addGroup(groupName, guids);
            } else if (command == "3") {
                std::string guid;
                std::cout << "Enter GUID: ";
                std::cin >> guid;
                auto name = getContactName(guid);
                if (name) {
                    std::cout << "Contact Name: " << *name << "\n";
                } else {
                    std::cout << "Contact not found.\n";
                }
            } else if (command == "4") {
                break;
            } else {
                std::cout << "Invalid option. Try again.\n";
            }
        }
    }

private:
    Logger& logger_;
    std::map<std::string, std::string> contacts_;
    std::map<std::string, std::vector<std::string>> groups_;
};

