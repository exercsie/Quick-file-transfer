#include "SendFile.h"
#include "../Data/Data.h"
#include "../Helpers/Helper.h"

#include <iostream>
#include <cstdint>
#include <sys/socket.h>
#include <print>
#include <filesystem>
#include <fstream>

Distribute d;

void sFile::sendFile(int socket, const std::string& path) {
    int bytesSend{};
    
    std::cout << "PATH IN SENDFILE IS: " << path << std::endl;

    // open file in binary
    FILE* file = fopen(path.c_str(), "rb");
    if(!file) {
        throw std::runtime_error("Failed to open file!");
    }

    // find file size
    fseek(file, 0, SEEK_END);
    std::size_t fileSize = ftell(file);
    std::cout << "FILE SIZE IS: " << fileSize << std::endl;
    fseek(file, 0, SEEK_SET);
    
    // find filename from path
    std::filesystem::path(path).filename().string();
    std::string fileName = std::filesystem::path(path).filename().string();
    std::cout << "FILE NAME IS " << fileName << std::endl;
    
    // send file size
    bytesSend = d.sendAll(socket, reinterpret_cast<char*>(&fileSize), sizeof(fileSize));
    if(bytesSend <= 0) {
        throw std::runtime_error("Failed to send file size!");
    }
    
    // send filename length
    int fileNameLength = fileName.size();
    bytesSend = d.sendAll(socket, reinterpret_cast<char*>(&fileNameLength), sizeof(fileNameLength));
    if(bytesSend <= 0) {
        throw std::runtime_error("Failed to send filename length!");
    }
    
    // send filename
    bytesSend = d.sendAll(socket, fileName.c_str(), fileNameLength);
    if(bytesSend <= 0) {
        throw std::runtime_error("Failed to send filename!");
    }

    // build the file's content
    buildFile(socket, file, fileSize, fileName);
}

void sFile::buildFile(int& socket, FILE* file, const std::size_t& fileSize, const std::string& fileName) {
    char buffer[BUFFERSIZE];
    std::size_t dataSent{};
    
    // align file contents in chunks
    std::println("\nSending {} of size {} bytes...", fileName, fileSize);
    while(dataSent < fileSize) {
        ssize_t readBytes = fread(buffer, 1, BUFFERSIZE, file);
        d.sendAll(socket, buffer, readBytes);
        dataSent += readBytes;
    }

    std::println("Sent {} of size {} bytes\n", fileName, fileSize);
    fclose(file);
}

bool sFile::createFile(const int& createFileChoice, std::string& path) {
    std::filesystem::perms::all;
    std::string fn;
    bool isEnteredCorrectly = true;
    while(true) {
        std::print("Enter file name: ");
        std::getline(std::cin, fn);
        std::filesystem::path fileName(fn);
        if(!(fileName.extension() == ".txt")) {
            std::println(stderr, "Error, {} must include .txt", fileName.string());
            continue;
        }

        if(std::filesystem::exists(fileName)) {
            char overwriteFile;
            bool backToWhileLoop = false;
            std::println("File already exists, do you wish to overwrite {}'s contents [Y/n]?", fileName.string());
            while(true) {
                std::cin >> overwriteFile;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                if(overwriteFile != 'Y' && overwriteFile != 'y' && overwriteFile != 'N' && overwriteFile != 'n') {
                    std::println("Invalid input!");
                    continue;
                }

                if(overwriteFile == 'Y' || overwriteFile == 'y') {
                    break;
                } 

                backToWhileLoop = true;
                break;
            }
            
            if(backToWhileLoop) {
                continue;
            }
        } 

        return writeContentsToFile(fileName.string(), path);
    }
}

bool sFile::writeContentsToFile(const std::filesystem::path& fileName, std::string& path) {
    std::ofstream file(fileName);
    std::string fileContents;
    std::print("Enter the contents to input into the file: ");
    std::getline(std::cin, fileContents);

    if(file.is_open()) {
        file << fileContents;
        std::println("Successfully added \"{}\" into {}", fileContents, fileName.string());
    } else {
        std::println(stderr, "File is closed!");
        return false;
    }

    std::string currentPath = std::filesystem::current_path();
    std::filesystem::path combined = currentPath / fileName;
    path = combined;
    std::cout << "PATH IS: " << path << std::endl;
    file.close();
    return true;
}