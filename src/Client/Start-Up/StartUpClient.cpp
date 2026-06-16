#include "StartUpClient.h"

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <print>

Client::~Client() {
    if(sock != -1) {
        close(sock);
    }
}

void Client::initialiseClientConnection() {
    createClientSocket();
    connectToServer();
}

void Client::createClientSocket() {
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == -1) {
        std::println(stderr, "Socket creation failed");
        exit(1);
    }

    //std::println("Socket created!");
}

void Client::connectToServer() {
    std::memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);

    int ipInt = inet_pton(AF_INET, ip.c_str(), &serverAddress.sin_addr);
    if(ipInt < 1) {
        std::println(stderr, "Invalid IP");
        exit(1);
    }

    int connectionEstablished = connect(sock, (sockaddr*)&serverAddress, sizeof(serverAddress));
    if(connectionEstablished == -1) {
        std::println(stderr, "Connection failed!");
        close(sock);
        exit(1);
    } 

    std::println("Connection established on IP: {} and on PORT: {}", ip, port);
}
