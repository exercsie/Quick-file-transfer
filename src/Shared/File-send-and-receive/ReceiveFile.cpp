#include "ReceiveFile.h"
#include "../Data/Data.h"
#include "../Helpers/Helper.h"

#include <iostream>
#include <sys/socket.h>
#include <string>
#include <print>

void rFile::receiveFile(int socket, const std::string& path) {
    Distribute d;
    char buffer[BUFFERSIZE];
    int bytesRec{};
    std::size_t fileSize{};

    // receive file size
    bytesRec = d.recvAll(socket, reinterpret_cast<char*>(&fileSize));
    if(bytesRec <= 0) {
        throw std::runtime_error("Failed to receive file size!");
    } 

    // receive filename length
    int fileNameLength;
    bytesRec = d.recvAll(socket, reinterpret_cast<char*>(&fileNameLength));
    if(bytesRec <= 0) {
        throw std::runtime_error("Failed to received filename length!");
    } 

    // receive fileName
    bytesRec = d.recvAll(socket, buffer);
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
    Distribute d;
    char buffer[BUFFERSIZE];
    int bytesRec;
    std::size_t dataRec{};

    std::println("\nReceiving {} of size {} bytes...", fileName, fileSize);
    while(dataRec < fileSize) {
        bytesRec = d.recvAll(socket, buffer);
        fwrite(buffer, 1, bytesRec, file);

        dataRec += bytesRec;
        if(dataRec == fileSize) {
            break;
        } 
    }

    std::println("Recevied {} of size {} bytes!", fileName, fileSize);
    fclose(file);
}