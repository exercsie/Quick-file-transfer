#pragma once

struct sFile {
    void sendFile(int& socket, const std::string& path);
    void buildFile(int& socket, FILE* file, const int& fileSize);
    void encryptFile();
};