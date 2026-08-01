#include "MenuServer.h"
#include "../Start-Up/StartUpServer.h"
#include "../../Shared/Data/Data.h"
#include "../../Shared/Send-and-receive/ReceiveFile.h"
#include "../../Shared/Send-and-receive/SendFile.h"
#include "../../Shared/Helpers/Helper.h"
#include "../../Shared/Send-and-receive/ReceiveDirectory.h"
#include "../../Shared/Send-and-receive/SendDirectory.h"

#include <iostream>
#include <unistd.h>
#include <format>
#include <filesystem>
#include <print>

void MenuServer::menuServer(Server &s, std::string& quickPath) {
    Distribute d;
    rFile rf;
    sFile sf;
    rDirectory rd;
    sDirectory sd;
    s.initialiseServerConnection();

    std::println("  ___        _      _    _____   _     _____                     __           ");
    std::println(" / _ \\ _   _(_) ___| | _|  ___(_) | __|_   _| __ __ _ _ __  ___ / _| ___ _ __ ");
    std::println("| | | | | | | |/ __| |/ / |_  | | |/ _ \\| || '__/ _` | '_ \\/ __| |_ / _ \\ '__|");
    std::println("| |_| | |_| | | (__|   <|  _| | | |  __/| || | | (_| | | | \\__ \\  _|  __/ |   ");
    std::println(" \\__\\_\\__,_ |_|\\___|_|\\_\\_|   |_|_|\\___||_||_|  \\__,_|_| |_|___ /_| \\___|_|   ");
    std::println();
    
    sleep(1);

    char buffer[BUFFERSIZE];
    std::size_t bytesSend{}, bytesRec{};
    bool isQuickPath = false;
    MessageType choice;

    while(true) {
        if(quickPath.length() > 1) {
            choice = MessageType::TYPE_SEND;
            isQuickPath = true;
        } else {
            std::println("0 - Exit");
            std::println("1 - Send a file");
            std::println("2 - Receive a file");
            std::cin >> choice;
            if(choice < MessageType::TYPE_EXIT || choice > MessageType::TYPE_RECEIVE) {
                std::println(stderr, "Invalid choice!");
                continue;
            }

            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        switch(choice) {
            MessageType type;
            case MessageType::TYPE_EXIT: {
                type = MessageType::TYPE_EXIT;
                bytesSend = d.sendAll(s.getClientFileDescriptor(), reinterpret_cast<char*>(&type), sizeof(type));
                close(s.getClientFileDescriptor());
                close(s.getServerFileDescriptor());
                std::println("Server closed!");
                exit(0);
                break;
            }

            // SERVER WANTS TO SEND
            case MessageType::TYPE_SEND: {
                type = MessageType::TYPE_SEND;
                bytesSend = d.sendAll(s.getClientFileDescriptor(), reinterpret_cast<char*>(&type), sizeof(type));
                
                if(isQuickPath) {
                    sendWithQuickPath(s, quickPath);
                }

                if(createFileOption(s)) {
                    continue;
                }

                while(true) {
                    std::string path;
                    std::print("Enter path: (Press Enter to go back) ");
                    std::getline(std::cin, path);
    
                    if(path.empty()) {
                        constexpr std::string_view goBack{":"};
                        d.sendAll(s.getClientFileDescriptor(), goBack.data(), goBack.size());
                        break;
                    }
                    
                    std::filesystem::path p(path);
                    if(!std::filesystem::exists(p)) {
                        std::println("Please input a valid path!");
                        continue;
                    }
    
                    // send path
                    d.sendAll(s.getClientFileDescriptor(), path.c_str(), path.size());

                    if(bool isDirectory = std::filesystem::is_directory(p)) {
                        // tell receiver that incoming file is directory
                        d.sendAll(s.getClientFileDescriptor(), reinterpret_cast<char*>(&isDirectory), sizeof(isDirectory));
                        sd.buildDirectory(s.getClientFileDescriptor(), p);
                        break;
                    } else {
                        d.sendAll(s.getClientFileDescriptor(), reinterpret_cast<char*>(&isDirectory), sizeof(isDirectory));
                    }

                    sf.sendFile(s.getClientFileDescriptor(), path);
                    break;
                }

                break;
            }

            // SERVER WANTS TO RECEIVE
            case MessageType::TYPE_RECEIVE: {
                type = MessageType::TYPE_RECEIVE;
                bytesRec = d.sendAll(s.getClientFileDescriptor(), reinterpret_cast<char*>(&type), sizeof(type));

                // receive path
                bytesRec = d.recvAll(s.getClientFileDescriptor(), buffer);
                const std::string path(buffer, bytesRec);
                if(path == ":") {
                    break;
                }

                bool isDirectory;
                bytesRec = d.recvAll(s.getClientFileDescriptor(), reinterpret_cast<char*>(&isDirectory));

                if(isDirectory) {
                    rd.buildDirectory(s.getClientFileDescriptor());
                    break;
                }

                rf.receiveFile(s.getClientFileDescriptor(), path);
                break;
            }
        }
    }

}

void MenuServer::sendWithQuickPath(Server &s, const std::string& quickPath) {
    sFile sf;
    sDirectory sd;
    Distribute d;
    
    std::size_t bytesSend{};
    MessageType type;
    bytesSend = d.sendAll(s.getClientFileDescriptor(), quickPath.c_str(), quickPath.size());
    sleep(1);
    
    std::filesystem::path p(quickPath);
    if(bool isDirectory = std::filesystem::is_directory(p)) {
        d.sendAll(s.getClientFileDescriptor(), reinterpret_cast<char*>(&isDirectory), sizeof(isDirectory));
        sd.buildDirectory(s.getClientFileDescriptor(), p);
        type = MessageType::TYPE_EXIT;
        d.sendAll(s.getClientFileDescriptor(), reinterpret_cast<char*>(&type), sizeof(type));
        close(s.getClientFileDescriptor());
        close(s.getServerFileDescriptor());
        std::println("Server closed!");
        exit(0);
    }

    bool isDirectory;
    d.sendAll(s.getClientFileDescriptor(), reinterpret_cast<char*>(&isDirectory), sizeof(isDirectory));

    sf.sendFile(s.getClientFileDescriptor(), quickPath);
    type = MessageType::TYPE_EXIT;
    d.sendAll(s.getClientFileDescriptor(), reinterpret_cast<char*>(&type), sizeof(type));
    close(s.getClientFileDescriptor());
    close(s.getServerFileDescriptor());
    std::println("Server closed!");
    exit(0);
}

const bool MenuServer::createFileOption(Server &s) {
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
            d.sendAll(s.getClientFileDescriptor(), customFilePath.c_str(), customFilePath.size());
            sleep(1);
            // tell receiver that the created file is not a directory
            bool isDirectory = false;
            d.sendAll(s.getClientFileDescriptor(), reinterpret_cast<char*>(&isDirectory), sizeof(isDirectory));
            sf.sendFile(s.getClientFileDescriptor(), customFilePath);
            return true;
        }
    }

    return false;
}