#include "StartUpServer.h"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <iostream>
#include <unistd.h>
#include <cstring>

void Server::initialiseServerConnection() {
    createServerFileDescriptor();
    bindSocket();
    listenOnServerFileDescriptor();
    acceptConnection();
}

Server::~Server() {
    if(serverFileDescriptor != -1) {
        close(serverFileDescriptor);
    }

    if(clientFileDescriptor != -1) {
        close(clientFileDescriptor);
    }
}

void Server::createServerFileDescriptor() {
    serverFileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    if(serverFileDescriptor == -1) {
        std::cerr << "Socket creation failed!\n";
        exit(1);
    }

    std::cout << "Socket created!\n";
}

void Server::bindSocket() {
    std::memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);

    int bindSock = bind(serverFileDescriptor, (sockaddr*)&serverAddress, sizeof(serverAddress));
    if(bindSock == -1) {
        std::cerr << "Bind failed!\n";
        exit(1);
    }

    std::cout << "Socket binded!\n";
}

void Server::listenOnServerFileDescriptor() {
    int listening = listen(serverFileDescriptor, 1);
    if(listening == -1) {
        std::cerr << "Listening failed!\n";
        exit(1);
    }

    std::cout << "Listening on PORT: " << port << "...\n";
}

void Server::acceptConnection() {
    sockaddr_in clientAddress{};
    socklen_t clientLength = sizeof(clientAddress);
    clientFileDescriptor = accept(serverFileDescriptor, (sockaddr*)&clientAddress, &clientLength);
    if(clientFileDescriptor == -1) {
        std::cerr << "Connection failed!\n";
        return;
    }

    std::cout << "Connection established on PORT: " << port << std::endl;;
}