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
    std::uint16_t folderNameSize{};
    std::uint32_t amountOfFiles{};

    // receive folder name size
    d.recvAll(socket, reinterpret_cast<char*>(&folderNameSize));

    // receive folder name
    bytesRec = d.recvAll(socket, buffer);
    std::string folderName(buffer, bytesRec);

    // receive amount of files
    d.recvAll(socket, reinterpret_cast<char*>(&amountOfFiles));

    // create directory and receive N amount of files
    std::filesystem::create_directory(folderName);
    for(std::size_t i{}; i < amountOfFiles; ++i) {
        rf.receiveFile(socket, folderName, isDirectory);
    }
}