#pragma once
#include <string>
#include <filesystem>

struct sFile {
    void sendFile(int socket, const std::string& path);
    void buildFile(int& socket, FILE* file, const std::size_t& fileSize, const std::string& fileName);
    void encryptFile();
    bool createFile(const int& createFileChoice, std::string& path);
    bool writeContentsToFile(const std::filesystem::path& fileName, std::string& path);
};