#include "StartUpServer.h"

#include <arpa/inet.h>
#include <sys/socket.h>
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <print>

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
        std::println(stderr, "Socket creation failed");
        exit(1);
    }

    std::println("Socket created!");
}

void Server::bindSocket() {
    std::memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);

    int bindSock = bind(serverFileDescriptor, (sockaddr*)&serverAddress, sizeof(serverAddress));
    if(bindSock == -1) {
        std::println("Bind failed");
        exit(1);
    }

    std::println("Socket binded!");
}

void Server::listenOnServerFileDescriptor() const {
    int listening = listen(serverFileDescriptor, 1);
    if(listening == -1) {
        std::println("Listening failed");
        exit(1);
    }

    std::println("Listening on PORT: {}", port);
}

void Server::acceptConnection() {
    sockaddr_in clientAddress{};
    socklen_t clientLength = sizeof(clientAddress);
    clientFileDescriptor = accept(serverFileDescriptor, (sockaddr*)&clientAddress, &clientLength);
    if(clientFileDescriptor == -1) {
        std::println(stderr, "Connection failed!");
        return;
    }

    std::println("Connection established on PORT: {}", port);
}