#include "MenuServer.h"
#include "../Start-Up/StartUpServer.h"
#include "../../Shared/Data/Data.h"
#include "../../Shared/File-send-and-receive/ReceiveFile.h"
#include "../../Shared/File-send-and-receive/SendFile.h"

#include <iostream>
#include <unistd.h>

void menuServer(const int& PORT, std::string& quickPath) {
    Server s(PORT);
    rFile rf;
    sFile sf;
    s.initialiseServerConnection();

    std::cout << "  ___        _      _    _____   _     _____                     __           \n";
    std::cout << " / _ \\ _   _(_) ___| | _|  ___(_) | __|_   _| __ __ _ _ __  ___ / _| ___ _ __ \n";
    std::cout << "| | | | | | | |/ __| |/ / |_  | | |/ _ \\| || '__/ _` | '_ \\/ __| |_ / _ \\ '__|\n";
    std::cout << "| |_| | |_| | | (__|   <|  _| | | |  __/| || | | (_| | | | \\__ \\  _|  __/ |   \n";
    std::cout << " \\__\\_\\__,_ |_|\\___|_|\\_\\_|   |_|_|\\___||_||_|  \\__,_|_| |_|___ /_| \\___|_|   \n";
    std::cout << std::endl;

    char buffer[BUFFERSIZE];
    int bytesSend{}, bytesRec{};
    bool isQuickPath = false;
    std::size_t choice;
    while(true) {
        if(quickPath.length() > 1) {
            choice = TYPE_SEND;
            isQuickPath = true;
        } else {
            std::cout << "0 - Exit\n";
            std::cout << "1 - Send a file\n";
            std::cout << "2 - Receive a file\n";
            std::cin >> choice;
            if(choice < 0 || choice > 2) {
                std::cerr << "Invalid choice!\n";
                continue;
            }

            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        switch(choice) {
            int type;
            case TYPE_EXIT: {
                type = TYPE_EXIT;
                bytesSend = send(s.getClientFileDescriptor(), &type, sizeof(type), 0);
                close(s.getClientFileDescriptor());
                close(s.getServerFileDescriptor());
                exit(0);
                break;
            }

            case TYPE_SEND: {
                type = TYPE_SEND;
                bytesSend = send(s.getClientFileDescriptor(), &type, sizeof(type), 0);
                
                if(isQuickPath) {
                    bytesSend = send(s.getClientFileDescriptor(), quickPath.c_str(), quickPath.length(), 0);
                    sf.sendFile(s.getClientFileDescriptor(), quickPath);
                    isQuickPath = false;
                    quickPath.clear();
                    continue;
                }

                std::string path;
                std::cout << "Enter path: ";
                std::getline(std::cin, path);

                // send path
                bytesSend = send(s.getClientFileDescriptor(), path.c_str(), path.length(), 0);

                sf.sendFile(s.getClientFileDescriptor(), path);
                break;
            }

            case TYPE_RECEIVE: {
                type = TYPE_RECEIVE;
                bytesSend = send(s.getClientFileDescriptor(), &type, sizeof(type), 0);

                // receive path
                bytesRec = recv(s.getClientFileDescriptor(), buffer, BUFFERSIZE, 0);
                std::string path(buffer, bytesRec);

                rf.receiveFile(s.getClientFileDescriptor(), path);
                break;
            }
        }
    }

}