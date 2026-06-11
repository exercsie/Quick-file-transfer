#include "StartUpClient.h"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <cstring>

Client::~Client() {
    if(sock == -1) {
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
        std::cerr << "Socket creation failed!\n";
        exit(1);
    }

    std::cout << "Socket created!\n";
}

void Client::defineIP() {
    int ipINT = inet_pton(AF_INET, ip.c_str(), &serverAddress.sin_addr);
    if(ipINT < 1) {
        std::cerr << "Invalid IP!\n";
        exit(1);
    }
}

void Client::connectToServer() {
    std::memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);

    int connectionEstablished = connect(sock, (sockaddr*)&serverAddress, sizeof(serverAddress));
    if(connectionEstablished == -1) {
        std::cerr << "Connection failed!\n";
        close(sock);
        exit(1);
    } 

    std::cout << "Connection established on IP: " << ip << " and PORT: " << port << std::endl;
}
