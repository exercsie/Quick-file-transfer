#include "SendDirectory.h"
#include "../Helpers/Helper.h"
#include "SendFile.h"

#include <iostream>
#include <filesystem>
#include <vector>

void sDirectory::buildDirectory(int socket, std::filesystem::path& p) {
    Distribute d;
    sFile sf;

    std::string path = p.string();
    std::uint16_t pathSize = path.size();

    std::vector<std::filesystem::path> files;
    for(const auto& it : std::filesystem::recursive_directory_iterator(p)) {
        if(it.is_regular_file()) {
            files.push_back(it.path());
        }
    }

    std::size_t amountOfFiles = files.size();

    // send direc name size
    d.sendAll(socket, reinterpret_cast<char*>(&pathSize), sizeof(pathSize));

    // send direc name
    d.sendAll(socket, path.c_str(), pathSize);

    // send amount of files in direc
    d.sendAll(socket, reinterpret_cast<char*>(&amountOfFiles), sizeof(amountOfFiles));

    // send files
    for(const auto& it : files) {
        sf.sendFile(socket, it.string());
    }
}

void sDirectory::sendDirectory(std::filesystem::path& p) {
    
}
