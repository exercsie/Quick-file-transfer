#include "SendDirectory.h"
#include "../Helpers/Helper.h"

#include <iostream>
#include <filesystem>

void sDirectory::buildDirectory(int socket, std::filesystem::path& p) {
    Distribute d;

    std::string path = p.string();
    std::uint16_t pathSize = path.size();
    std::cout << "DIREC SIZE IS " << pathSize << std::endl;
    std::size_t amountOfFiles{};

    // send direc name size
    d.sendAll(socket, reinterpret_cast<char*>(&pathSize), sizeof(pathSize));

    // send direc name
    d.sendAll(socket, path.c_str(), pathSize);

    // count files
    for(const auto& it : std::filesystem::recursive_directory_iterator(p)) {
        ++amountOfFiles;
    }
    
    // send amount of files in direc
    d.sendAll(socket, reinterpret_cast<char*>(&amountOfFiles), sizeof(amountOfFiles));
}

void sDirectory::sendDirectory(std::filesystem::path& p) {
    
}
