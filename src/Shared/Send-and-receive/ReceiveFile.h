#pragma once
#include <string>


struct rFile {
    std::size_t receiveFile(int socket, const std::string& path, bool isDirectory = false);
    void buildFile(int& socket, FILE* file, const size_t& fileSize, const std::string& fileName);
    void decryptFile();
};