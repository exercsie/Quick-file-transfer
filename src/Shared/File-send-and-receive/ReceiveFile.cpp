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
    std::println("\nReceiving file size!");
    bytesRec = d.recvAll(socket, reinterpret_cast<char*>(&fileSize));
    if(bytesRec <= 0) {
        throw std::runtime_error("Failed to receive file size!");
    } else {
        std::println("Received file size {}", fileSize);
    }

    // receive filename length
    std::println("\nReciving filename length!");
    int fileNameLength;
    bytesRec = d.recvAll(socket, reinterpret_cast<char*>(&fileNameLength));
    if(bytesRec <= 0) {
        throw std::runtime_error("Failed to received filename length!");
    } else {
        std::println("Received filename length: {}", fileNameLength);
    }

    // receive fileName
    std::println("\nReceiving filename!");
    bytesRec = d.recvAll(socket, buffer);
    std::string fileName(buffer, bytesRec);

    if(bytesRec <= 0) {
        throw std::runtime_error("Failed to receive filename!");
    } else {
        std::cout << "file name: " << fileName << std::endl;
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
        std::size_t readBytes = (fileSize - dataRec > BUFFERSIZE) ? BUFFERSIZE : fileSize - dataRec;

        std::cout << "Waiting for recv..\n";
        bytesRec = d.recvAll(socket, buffer);
        std::cout << "After waiting for rec..\n";
        fwrite(buffer, 1, bytesRec, file);

        dataRec += bytesRec;
        std::println("Datarec: {} fileSize: {}", dataRec, fileSize);
        if(dataRec == fileSize) {
            break;
        } else {
            std::cout << "nope!\n";
        }
    }

    std::println("Recevied {} of size {} bytes!", fileName, fileSize);
    fclose(file);
}