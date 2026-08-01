#include "ReceiveDirectory.h"
#include "../Helpers/Helper.h"
#include "../Data/Data.h"

#include <filesystem>
#include <iostream>
#include <string>

void rDirectory::buildDirectory(int socket) {
    Distribute d;
    std::size_t bytesRec{};
    char buffer[BUFFERSIZE];
    std::uint16_t fileNameSize{};
    std::uint32_t amountOfFiles{};

    d.recvAll(socket, reinterpret_cast<char*>(&fileNameSize));

    bytesRec = d.recvAll(socket, buffer);
    std::string fileName(buffer, bytesRec);

    d.recvAll(socket, reinterpret_cast<char*>(&amountOfFiles));
}