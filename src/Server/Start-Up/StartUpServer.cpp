#include "StartUpServer.h"

#include <arpa/inet.h>
#include <sys/socket.h>
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <print>

void Server::initialiseServerConnection() noexcept {
    createServerFileDescriptor();
    bindSocket();
    listenOnServerFileDescriptor();
    acceptConnection();
}

Server::~Server() noexcept {
    if(serverFileDescriptor != -1) {
        close(serverFileDescriptor);
    }

    if(clientFileDescriptor != -1) {
        close(clientFileDescriptor);
    }
}

void Server::createServerFileDescriptor() noexcept {
    serverFileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    if(serverFileDescriptor == -1) {
        std::println(std::cerr, "Socket creation failed");
        std::exit(1);
    }

    //std::println("Socket created!");
}

void Server::bindSocket() noexcept {
    std::memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);

    int bindSock = bind(serverFileDescriptor, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress));
    if(bindSock == -1) {
        std::println(std::cerr, "Bind failed");
        std::exit(1);
    }

    //std::println("Socket binded!");
}

void Server::listenOnServerFileDescriptor() noexcept {
    int listening = listen(serverFileDescriptor, 1);
    if(listening == -1) {
        std::println(std::cerr, "Listening failed");
        std::exit(1);
    }

    std::println("Listening on PORT: {}", port);
}

void Server::acceptConnection() noexcept {
    sockaddr_in clientAddress{};
    socklen_t clientLength = sizeof(clientAddress);
    clientFileDescriptor = accept(serverFileDescriptor, reinterpret_cast<sockaddr*>(&clientAddress), &clientLength);
    if(clientFileDescriptor == -1) {
        std::println(std::cerr, "Connection failed!");
        return;
    }

    std::println("Connection established on PORT: {}", port);
}