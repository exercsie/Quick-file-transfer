#pragma once

#include <iostream>
#include <filesystem>

struct sDirectory {
    void buildDirectory(int socket, std::filesystem::path& p);
    void sendDirectory(std::filesystem::path& p);
};