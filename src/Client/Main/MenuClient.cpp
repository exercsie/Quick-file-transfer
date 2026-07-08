#include "MenuClient.h"
#include "../Start-Up/StartUpClient.h"
#include "../../Shared/Data/Data.h"
#include "../../Shared/File-send-and-receive/ReceiveFile.h"
#include "../../Shared/File-send-and-receive/SendFile.h"
#include "../../Shared/Helpers/Helper.h"

#include <iostream>
#include <string>
#include <unistd.h>
#include <filesystem>
#include <print>

void menuClient(Client& c) {
    Distribute d;
    rFile rf;
    sFile sf;
    c.initialiseClientConnection();

    std::println("  ___        _      _    _____   _     _____                     __           ");
    std::println(" / _ \\ _   _(_) ___| | _|  ___(_) | __|_   _| __ __ _ _ __  ___ / _| ___ _ __ ");
    std::println("| | | | | | | |/ __| |/ / |_  | | |/ _ \\| || '__/ _` | '_ \\/ __| |_ / _ \\ '__|");
    std::println("| |_| | |_| | | (__|   <|  _| | | |  __/| || | | (_| | | | \\__ \\  _|  __/ |   ");
    std::println(" \\__\\_\\__,_ |_|\\___|_|\\_\\_|   |_|_|\\___||_||_|  \\__,_|_| |_|___ /_| \\___|_|   ");
    std::println();

    sleep(1);

    char buffer[BUFFERSIZE];
    int bytesRec{};
    int bytesSend{};
    while(true) {
        int type;
        bytesRec = d.recvAll(c.getClientSocket(), reinterpret_cast<char*>(&type));
        switch(type) {
            case TYPE_EXIT: {
                std::println(stderr, "Server closed by host!");
                close(c.getClientSocket());
                exit(0);
                break;
            }

            // SERVER WANTS TO SEND
            case TYPE_SEND: {
                // receive path
                bytesRec = d.recvAll(c.getClientSocket(), buffer);
                const std::string path(buffer, bytesRec);
                if(path == "goBack") {
                    break;
                }

                rf.receiveFile(c.getClientSocket(), path);
                break;
            }

            // SERVER WANTS TO RECEIVE
            case TYPE_RECEIVE: {
                if(createFileOption(c)) {
                    continue;
                }

                while(true) {
                    std::string path;
                    std::print("Enter path: (Press Enter to go back) ");
                    std::getline(std::cin, path);
    
                    if(path.empty()) {
                        constexpr std::string_view goBack{"goBack"};
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
    
                    sf.sendFile(c.getClientSocket(), path);
                    break;
                }

                break;
            }
        }
    }
}

const bool createFileOption(Client& c) {
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
            sf.sendFile(c.getClientSocket(), customFilePath.c_str());
            return true;
        }
    }

    return false;
}