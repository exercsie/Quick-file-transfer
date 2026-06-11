#pragma once
#include <string>

struct sFile {
    void sendFile(int socket, const std::string& path);
    void buildFile(int& socket, FILE* file, const std::size_t& fileSize, const std::string& fileName);
    void encryptFile();
};