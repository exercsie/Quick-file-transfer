#include "ReceiveFile.h"
#include "../Data/Data.h"
#include <iostream>
#include <sys/socket.h>
#include <string>
#include <format>

void rFile::receiveFile(int socket, const std::string& path) {
    char buffer[BUFFERSIZE];
    int bytesRec{};
    std::size_t fileSize{};

    // receive file size
    bytesRec = recv(socket, (char*)&fileSize, sizeof(fileSize), 0);
    if(bytesRec <= 0) {
        throw std::runtime_error("Failed to receive file size!");
    } 

    // receive fileName
    bytesRec = recv(socket, buffer, BUFFERSIZE, 0);
    std::string fileName(buffer, bytesRec);

    if(bytesRec <= 0) {
        throw std::runtime_error("Failed to receive filename!");
    }


    FILE* file = fopen(fileName.c_str(), "wb");
    if(!file) {
        throw std::runtime_error("Failed to create file!");
    }

    buildFile(socket, file, fileSize, fileName);
}

void rFile::buildFile(int& socket, FILE* file, const std::size_t& fileSize, const std::string& fileName) {
    char buffer[BUFFERSIZE];
    int bytesRec;
    std::size_t dataRec{};

    std::string receivingFile { std::format("Receiving {} of size {} bytes...", fileName, fileSize) };
    std::cout << receivingFile << std::endl;
    while(dataRec < fileSize) {
        std::size_t readBytes = (fileSize - dataRec > BUFFERSIZE) ? BUFFERSIZE : fileSize - dataRec;

        bytesRec = recv(socket, buffer, readBytes, 0);
        fwrite(buffer, 1, bytesRec, file);

        dataRec += bytesRec;
    }

    std::string receivedFile { std::format("Received {} of size {} bytes!", fileName, fileSize) };
    std::cout << receivedFile << std::endl;
    fclose(file);
}