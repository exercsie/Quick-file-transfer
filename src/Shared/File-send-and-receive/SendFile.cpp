#include "SendFile.h"
#include "../Data/Data.h"
#include <iostream>
#include <cstdint>
#include <sys/socket.h>

void sFile::sendFile(int& socket, const std::string& path) {
    int bytesSend;
    FILE* file = fopen(path.c_str(), "rb");

    fseek(file, 0, SEEK_END);
    std::size_t fileSize = ftell(file);
    fseek(file, 0, SEEK_END);

    std::string fileName;
    size_t p = path.find_last_of("/\\");
    if(p != std::string::npos) {
        fileName = path.substr(p + 1);
    } else {
        fileName = path;
    }

    bytesSend = send(socket, &fileSize, sizeof(fileSize), 0);
    if(bytesSend <= 0) {
        throw std::runtime_error("Failed to send file size!");
    }

    bytesSend = send(socket, fileName.c_str(), fileName.size(), 0);
    if(bytesSend <= 0) {
        throw std::runtime_error("Failed to send filename!");
    }

    buildFile(socket, file, fileSize);
}

void sFile::buildFile(int& socket, FILE* file, const int& fileSize) {
    char buffer[BUFFERSIZE];
    int dataSent{};
    
    while(dataSent < fileSize) {
        std::size_t readBytes = fread(buffer, 1, BUFFERSIZE, file);
        send(socket, buffer, readBytes, 0);
        dataSent += readBytes;
    }

    std::cout << "File sent!\n";
}