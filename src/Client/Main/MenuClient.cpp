#include "MenuClient.h"
#include "../Start-Up/StartUpClient.h"
#include "../../Shared/Data/Data.h"
#include "../../Shared/Send-and-receive/ReceiveFile.h"
#include "../../Shared/Send-and-receive/SendFile.h"
#include "../../Shared/Helpers/Helper.h"
#include "../../Shared/Send-and-receive/ReceiveDirectory.h"
#include "../../Shared/Send-and-receive/SendDirectory.h"

#include <iostream>
#include <string>
#include <unistd.h>
#include <filesystem>
#include <print>

void MenuClient::menuClient(Client& c) {
    Distribute d;
    rFile rf;
    sFile sf;
    rDirectory rd;
    sDirectory sd;
    c.initialiseClientConnection();

    std::println("  ___        _      _    _____   _     _____                     __           ");
    std::println(" / _ \\ _   _(_) ___| | _|  ___(_) | __|_   _| __ __ _ _ __  ___ / _| ___ _ __ ");
    std::println("| | | | | | | |/ __| |/ / |_  | | |/ _ \\| || '__/ _` | '_ \\/ __| |_ / _ \\ '__|");
    std::println("| |_| | |_| | | (__|   <|  _| | | |  __/| || | | (_| | | | \\__ \\  _|  __/ |   ");
    std::println(" \\__\\_\\__,_ |_|\\___|_|\\_\\_|   |_|_|\\___||_||_|  \\__,_|_| |_|___ /_| \\___|_|   ");
    std::println();

    sleep(1);

    char buffer[BUFFERSIZE];
    std::size_t bytesRec{}, bytesSend{};
    while(true) {
        MessageType type;
        bytesRec = d.recvAll(c.getClientSocket(), reinterpret_cast<char*>(&type));
        switch(type) {
            case MessageType::TYPE_EXIT: {
                close(c.getClientSocket());
                std::println("Server closed by host!");
                exit(0);
                break;
            }

            // SERVER WANTS TO SEND
            case MessageType::TYPE_SEND: {
                // receive path
                bytesRec = d.recvAll(c.getClientSocket(), buffer);
                const std::string path(buffer, bytesRec);
                if(path == ":") {
                    break;
                }

                bool isDirectory;
                bytesRec = d.recvAll(c.getClientSocket(), reinterpret_cast<char*>(&isDirectory));
                //std::cout << std::boolalpha << isDirectory << std::endl;

                if(isDirectory) {
                    rd.buildDirectory(c.getClientSocket());
                    break;
                }

                rf.receiveFile(c.getClientSocket(), path);
                break;
            }

            // SERVER WANTS TO RECEIVE
            case MessageType::TYPE_RECEIVE: {
                if(createFileOption(c)) {
                    continue;
                }

                while(true) {
                    std::string path;
                    std::print("Enter path: (Press Enter to go back) ");
                    std::getline(std::cin, path);
    
                    if(path.empty()) {
                        constexpr std::string_view goBack{":"};
                        d.sendAll(c.getClientSocket(), goBack.data(), goBack.size());
                        break;
                    }

                    std::filesystem::path p(path);
                    if(!std::filesystem::exists(p)) {
                        std::println(stderr, "Please input a valid path!\n");
                        continue;
                    }
    
                    // send path
                    bytesSend = d.sendAll(c.getClientSocket(), path.c_str(), path.size());
    
                    if(bool isDirectory = std::filesystem::is_directory(p)) {
                        // tell receiver that incoming file is directory
                        d.sendAll(c.getClientSocket(), reinterpret_cast<char*>(&isDirectory), sizeof(isDirectory));
                        sd.buildDirectory(c.getClientSocket(), p);
                        break;
                    } else {
                        d.sendAll(c.getClientSocket(), reinterpret_cast<char*>(&isDirectory), sizeof(isDirectory));
                    }

                    sf.sendFile(c.getClientSocket(), path);
                    break;
                }

                break;
            }
        }
    }
}

const bool MenuClient::createFileOption(Client& c) {
    Distribute d;
    rFile rf;
    sFile sf;
    char createFileChoice;
    while(true) {
        std::println("Do you want to create a file to send? [Y/n]");
        std::cin >> createFileChoice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if(createFileChoice != 'Y' && createFileChoice != 'y' && createFileChoice != 'n' && createFileChoice != 'N') {
            std::println(stderr, "Invalid choice!");
            continue;
        }

        break;
    }

    std::string customFilePath;
    if(createFileChoice == 'Y' || createFileChoice == 'y') {
        bool isCreateFile = sf.createFile(createFileChoice, customFilePath);
        if(isCreateFile) {
            d.sendAll(c.getClientSocket(), customFilePath.c_str(), customFilePath.size());
            sleep(1);
            bool isDirectory = false;
            d.sendAll(c.getClientSocket(), reinterpret_cast<char*>(&isDirectory), sizeof(isDirectory));
            sf.sendFile(c.getClientSocket(), customFilePath.c_str());
            return true;
        }
    }

    return false;
}