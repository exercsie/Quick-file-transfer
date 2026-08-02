#include "StartUpClient.h"

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <print>

Client::~Client() noexcept {
    if(sock != -1) {
        close(sock);
    }
}

void Client::initialiseClientConnection() noexcept {
    createClientSocket();
    connectToServer();
}

void Client::createClientSocket() noexcept {
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == -1) {
        std::println(std::cerr, "Socket creation failed");
        std::exit(1);
    }

    //std::println("Socket created!");
}

void Client::connectToServer() noexcept {
    std::memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);

    int ipInt = inet_pton(AF_INET, ip.c_str(), &serverAddress.sin_addr);
    if(ipInt < 1) {
        std::println(std::cerr, "Invalid IP");
        std::exit(1);
    }

    int connectionEstablished = connect(sock, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress));
    if(connectionEstablished == -1) {
        std::println(std::cerr, "Connection failed!");
        close(sock);
        std::exit(1);
    } 

    std::println("Connection established on IP: {} and on PORT: {}", ip, port);
}
