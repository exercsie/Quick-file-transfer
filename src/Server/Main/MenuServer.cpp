#include "MenuServer.h"
#include "../Start-Up/StartUpServer.h"
#include "../../Shared/Data/Data.h"
#include "../../Shared/File-send-and-receive/ReceiveFile.h"
#include "../../Shared/File-send-and-receive/SendFile.h"
#include "../../Shared/Helpers/Helper.h"

#include <iostream>
#include <unistd.h>
#include <format>
#include <filesystem>
#include <print>

void menuServer(Server &s, std::string& quickPath) {
    Distribute d;
    rFile rf;
    sFile sf;
    s.initialiseServerConnection();

    std::println("  ___        _      _    _____   _     _____                     __           ");
    std::println(" / _ \\ _   _(_) ___| | _|  ___(_) | __|_   _| __ __ _ _ __  ___ / _| ___ _ __ ");
    std::println("| | | | | | | |/ __| |/ / |_  | | |/ _ \\| || '__/ _` | '_ \\/ __| |_ / _ \\ '__|");
    std::println("| |_| | |_| | | (__|   <|  _| | | |  __/| || | | (_| | | | \\__ \\  _|  __/ |   ");
    std::println(" \\__\\_\\__,_ |_|\\___|_|\\_\\_|   |_|_|\\___||_||_|  \\__,_|_| |_|___ /_| \\___|_|   ");
    std::println();
    
    sleep(1);

    char buffer[BUFFERSIZE];
    int bytesSend{}, bytesRec{};
    bool isQuickPath = false;
    std::size_t choice;
    while(true) {
        if(quickPath.length() > 1) {
            choice = TYPE_SEND;
            isQuickPath = true;
        } else {
            std::println("0 - Exit");
            std::println("1 - Send a file");
            std::println("2 - Receive a file");
            std::cin >> choice;
            if(choice < 0 || choice > 2) {
                std::println(stderr, "Invalid choice!");
                continue;
            }

            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        switch(choice) {
            int type;
            case TYPE_EXIT: {
                type = TYPE_EXIT;
                bytesSend = d.sendAll(s.getClientFileDescriptor(), reinterpret_cast<char*>(&type), sizeof(type));
                close(s.getClientFileDescriptor());
                close(s.getServerFileDescriptor());
                exit(0);
                break;
            }

            // SERVER WANTS TO SEND
            case TYPE_SEND: {
                type = TYPE_SEND;
                bytesSend = d.sendAll(s.getClientFileDescriptor(), reinterpret_cast<char*>(&type), sizeof(type));
                
                if(isQuickPath) {
                    bytesSend = d.sendAll(s.getClientFileDescriptor(), quickPath.c_str(), quickPath.size());
                    sleep(1);
                    sf.sendFile(s.getClientFileDescriptor(), quickPath);
                    isQuickPath = false;
                    quickPath.clear();
                    continue;
                }

                if(createFileOption(s)) {
                    continue;
                }

                std::string path;
                std::print("Enter path: ");
                std::getline(std::cin, path);

                std::filesystem::path p(path);
                if(path.empty() || !std::filesystem::exists(p)) {
                    std::string error = "pathError";
                    bytesSend = d.sendAll(s.getClientFileDescriptor(), error.c_str(), error.size());
                    std::println("Please input a valid path!");
                    break;
                }

                // send path
                d.sendAll(s.getClientFileDescriptor(), path.c_str(), path.size());

                sf.sendFile(s.getClientFileDescriptor(), path);
                break;
            }

            // SERVER WANTS TO RECEIVE
            case TYPE_RECEIVE: {
                type = TYPE_RECEIVE;
                bytesRec = d.sendAll(s.getClientFileDescriptor(), reinterpret_cast<char*>(&type), sizeof(type));

                // receive path
                bytesRec = d.recvAll(s.getClientFileDescriptor(), buffer);
                std::string path(buffer, bytesRec);
                if(path == "pathError") {
                    std::println(stderr, "Error code: \"{}\". Client failed to input a valid path!", path);
                    break;
                }

                rf.receiveFile(s.getClientFileDescriptor(), path);
                break;
            }
        }
    }

}

bool createFileOption(Server &s) {
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
            sf.sendFile(s.getClientFileDescriptor(), customFilePath);
            return true;
        }
    }

    return false;
}