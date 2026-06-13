#include "SendFile.h"
#include "../Data/Data.h"
#include "../Helpers/Helper.h"

#include <iostream>
#include <cstdint>
#include <sys/socket.h>
#include <print>

Distribute d;

void sFile::sendFile(int socket, const std::string& path) {
    int bytesSend{};
    
    // open file in binary
    FILE* file = fopen(path.c_str(), "rb");
    if(!file) {
        throw std::runtime_error("Failed to open file!");
    }

    // find file size
    fseek(file, 0, SEEK_END);
    std::size_t fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    // find filename from path
    std::string fileName;
    std::size_t p = path.find_last_of("/\\");
    if(p != std::string::npos) {
        fileName = path.substr(p + 1);
    } else {
        fileName = path;
    }
    
    // send file size
    std::println("\nSending file size!");
    bytesSend = d.sendAll(socket, reinterpret_cast<char*>(&fileSize), sizeof(fileSize));
    if(bytesSend <= 0) {
        throw std::runtime_error("Failed to send file size!");
    } else {
        std::println("Sent file size {}", fileSize);
    }

    // send filename length
    std::println("\nSending filename length!");
    int fileNameLength = fileName.size();
    bytesSend = d.sendAll(socket, reinterpret_cast<char*>(&fileNameLength), sizeof(fileNameLength));
    if(bytesSend <= 0) {
        throw std::runtime_error("Failed to send filename length!");
    } else {
        std::println("Sent filename length of {}", fileNameLength);
    }

    // send filename
    std::println("\nSending filename!");
    bytesSend = d.sendAll(socket, fileName.c_str(), fileNameLength);
    if(bytesSend <= 0) {
        throw std::runtime_error("Failed to send filename!");
    } else {
        std::println("Sent filename: {}", fileName);
    }

    // build the file's content
    buildFile(socket, file, fileSize, fileName);
}

void sFile::buildFile(int& socket, FILE* file, const std::size_t& fileSize, const std::string& fileName) {
    char buffer[BUFFERSIZE];
    std::size_t dataSent{};
    
    // align file contents in chunks
    std::println("\nSending {} of size {} bytes...", fileName, fileSize);
    while(dataSent < fileSize) {
        ssize_t readBytes = fread(buffer, 1, BUFFERSIZE, file);
        d.sendAll(socket, buffer, readBytes);
        dataSent += readBytes;
    }

    std::println("Sent {} of size {} bytes....\n", fileName, fileSize);
    fclose(file);
}