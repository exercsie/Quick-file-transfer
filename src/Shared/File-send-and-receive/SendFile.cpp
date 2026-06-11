#include "SendFile.h"
#include "../Data/Data.h"
#include <iostream>
#include <cstdint>
#include <sys/socket.h>

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
    bytesSend = send(socket, &fileSize, sizeof(fileSize), 0);
    if(bytesSend <= 0) {
        throw std::runtime_error("Failed to send file size!");
    }

    // send filename
    bytesSend = send(socket, fileName.c_str(), fileName.length(), 0);
    if(bytesSend <= 0) {
        throw std::runtime_error("Failed to send filename!");
    }

    // build the file's content
    buildFile(socket, file, fileSize, fileName);
}

void sFile::buildFile(int& socket, FILE* file, const std::size_t& fileSize, const std::string& fileName) {
    char buffer[BUFFERSIZE];
    std::size_t dataSent{};
    
    // align file contents in chunks
    std::cout << "Sending \"" << fileName << "\" of size " << fileSize << "...\n";
    while(dataSent < fileSize) {
        std::size_t readBytes = fread(buffer, 1, BUFFERSIZE, file);
        send(socket, buffer, readBytes, 0);
        dataSent += readBytes;
    }

    std::cout << "File sent!\n";
    fclose(file);
}