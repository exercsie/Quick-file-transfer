#include "StartUpServer.h"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <iostream>

void Server::createServerFileDescriptor() {
    serverFileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    if(serverFileDescriptor == -1) {
        std::cout << "Socket failed!\n";
        exit(1);
    }

    std::cout << "Socket created!\n";
}

int Server::getServerFileDescriptorSocket() const {
    return serverFileDescriptor;
}

void Server::bindSocket(const int& PORT) {
    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(PORT);

    int bindSock = bind(serverFileDescriptor, (sockaddr*)(&serverAddress), sizeof(serverAddress));
    if(bindSock == -1) {
        std::cout << "Bind failed!\n";
    }

    std::cout << "Socket binded!\n";
}

void Server::listenOnServerFileDescriptor(const int& PORT) {
    int listening = listen(serverFileDescriptor, 1);
    if(listening == -1) {
        std::cout << "Listening failed!\n";
    }

    std::cout << "Listening on PORT: " << PORT << "...";
}

