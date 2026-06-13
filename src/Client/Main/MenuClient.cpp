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
                std::string path(buffer, bytesRec);
                if(path == "pathError") {
                    std::println(stderr, "Error code: \"{}\". Server failed to input a valid path!", path);
                    break;
                } 

                rf.receiveFile(c.getClientSocket(), path);
                break;
            }

            // SERVER WANTS TO RECEIVE
            case TYPE_RECEIVE: {
                std::string path;
                std::print("Enter path: ");
                std::getline(std::cin, path);

                std::filesystem::path p(path);
                if(path.empty() || !std::filesystem::exists(p)) {
                    std::string error = "pathError";
                    bytesSend = d.sendAll(c.getClientSocket(), error.c_str(), error.size());
                    std::println(stderr, "Please input a valid path!\n");
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