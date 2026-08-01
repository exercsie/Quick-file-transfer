#include "ReceiveDirectory.h"
#include "ReceiveFile.h"
#include "../Helpers/Helper.h"
#include "../Data/Data.h"

#include <filesystem>
#include <iostream>
#include <string>


void rDirectory::buildDirectory(int socket) {
    bool isDirectory = true;
    Distribute d;
    rFile rf;

    std::size_t bytesRec{};
    char buffer[BUFFERSIZE];
    std::uint8_t directoryNameSize{};
    
    // receive directory name size
    bytesRec = d.recvAll(socket, reinterpret_cast<char*>(&directoryNameSize));
    if(bytesRec <= 0) {
        throw std::runtime_error("Failed to receive folder name size!");
    }
    
    // receive directory name
    bytesRec = d.recvAll(socket, buffer);
    if(bytesRec <= 0) {
        throw std::runtime_error("Failed to receive folder name!");
    }
    
    std::string directoryName(buffer, directoryNameSize);
    
    // receive amount of files
    std::uint32_t amountOfFiles{};
    bytesRec = d.recvAll(socket, reinterpret_cast<char*>(&amountOfFiles));
    if(bytesRec <= 0) {
        throw std::runtime_error("Failed to receive amount of files!");
    }

    // create directory and receive N amount of files
    std::filesystem::create_directory(directoryName);
    for(std::uint32_t i{}; i < amountOfFiles; ++i) {
        rf.receiveFile(socket, directoryName, isDirectory);
    }
}