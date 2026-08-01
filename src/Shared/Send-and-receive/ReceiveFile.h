#pragma once
#include <string>


struct rFile {
    void receiveFile(int socket, const std::string& path);
    void buildFile(int& socket, FILE* file, const size_t& fileSize, const std::string& fileName);
    void decryptFile();
};