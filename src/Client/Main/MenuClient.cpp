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

void menuClient(std::string& IP, const int& PORT) {
    Distribute d;
    Client c(IP, PORT);
    rFile rf;
    sFile sf;
    c.initialiseClientConnection();

    std::cout << "  ___        _      _    _____   _     _____                     __           \n";
    std::cout << " / _ \\ _   _(_) ___| | _|  ___(_) | __|_   _| __ __ _ _ __  ___ / _| ___ _ __ \n";
    std::cout << "| | | | | | | |/ __| |/ / |_  | | |/ _ \\| || '__/ _` | '_ \\/ __| |_ / _ \\ '__|\n";
    std::cout << "| |_| | |_| | | (__|   <|  _| | | |  __/| || | | (_| | | | \\__ \\  _|  __/ |   \n";
    std::cout << " \\__\\_\\__,_ |_|\\___|_|\\_\\_|   |_|_|\\___||_||_|  \\__,_|_| |_|___ /_| \\___|_|   \n";
    std::cout << std::endl;

    std::cout << "Awaiting Server's input..\n";

    char buffer[BUFFERSIZE];
    int bytesRec{};
    int bytesSend{};
    while(true) {
        int type;
        std::println("Type is: {}", type);
        bytesRec = d.recvAll(c.getClientSocket(), reinterpret_cast<char*>(&type));
        std::println("Type is: {}", type);
        switch(type) {
            case TYPE_EXIT: {
                std::cerr << "Server closed by host!\n";
                close(c.getClientSocket());
                exit(0);
                break;
            }

            // SERVER WANTS TO SEND
            case TYPE_SEND: {
                // receive path
                bytesRec = d.recvAll(c.getClientSocket(), buffer);
                std::string path(buffer, bytesRec);
                if(path == "error") {
                    break;
                } 

                rf.receiveFile(c.getClientSocket(), path);
                break;
            }

            // SERVER WANTS TO RECEIVE
            case TYPE_RECEIVE: {
                std::string path;
                std::cout << "Enter path: ";
                std::getline(std::cin, path);

                std::filesystem::path p(path);
                if(path.empty() || !std::filesystem::exists(p)) {
                    std::string error = "error";
                    bytesSend = d.sendAll(c.getClientSocket(), error.c_str(), error.size());
                    std::cerr << "Please input a valid path!\n";
                    break;
                }

                // send path
                bytesSend = d.sendAll(c.getClientSocket(), path.c_str(), path.size());

                sf.sendFile(c.getClientSocket(), path);
                break;
            }
        }
    }
}