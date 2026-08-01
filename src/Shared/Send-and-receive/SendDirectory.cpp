#include "SendDirectory.h"
#include "../Helpers/Helper.h"
#include "SendFile.h"

#include <iostream>
#include <filesystem>
#include <vector>
#include <print>

void sDirectory::buildDirectory(int socket, std::filesystem::path& p) {
    Distribute d;
    sFile sf;

    std::size_t bytesSend{};
    std::string directoryName = p.string();
    std::uint8_t directoryNameSize = directoryName.size();

    std::vector<std::filesystem::path> files;
    for(const auto& it : std::filesystem::recursive_directory_iterator(p)) {
        if(it.is_regular_file()) {
            files.push_back(it.path());
        }
    }
    
    // send direc name size
    bytesSend = d.sendAll(socket, reinterpret_cast<char*>(&directoryNameSize), sizeof(directoryNameSize));
    if(bytesSend <= 0) {
        throw std::runtime_error("Failed to send directory name size!");
    }
    
    // send direc name
    bytesSend = d.sendAll(socket, directoryName.c_str(), directoryNameSize);
    if(bytesSend <= 0) {
        throw std::runtime_error("Failed to send directory name!");
    }
    
    // send amount of files in direc
    std::uint32_t amountOfFiles = files.size();
    bytesSend = d.sendAll(socket, reinterpret_cast<char*>(&amountOfFiles), sizeof(amountOfFiles));
    if(bytesSend <= 0) {
        throw std::runtime_error("Failed to send amount of files!");
    }

    std::size_t totalBytesSent{};

    // send files
    for(const auto& it : files) {
        totalBytesSent += sf.sendFile(socket, it.string());
    }

    std::println("\nSent {} files of total size {} bytes!", amountOfFiles, totalBytesSent);
}