#pragma once

#include <arpa/inet.h>
#include <cstdint>

class Server {
public:
    // constructor
    explicit Server(std::uint16_t p) noexcept : serverAddress{}, serverFileDescriptor(-1), clientFileDescriptor(-1), port(p) {}

    // destructor
    ~Server() noexcept;

    // wrapper
    void initialiseServerConnection() noexcept;

    // base server creators
    void createServerFileDescriptor() noexcept;
    void bindSocket() noexcept;
    void listenOnServerFileDescriptor() noexcept;
    void acceptConnection() noexcept;

    // getters
    [[nodiscard]] int getClientFileDescriptor() const noexcept{ return clientFileDescriptor; }
    [[nodiscard]] int getServerFileDescriptor() const noexcept{ return serverFileDescriptor; }
private:
    // sort from largest to smallest size so memory address can 
    // be divided by type, thus compiler doesn't allocate wasted bytes
    // https://ibb.co/r2gCZQ8M
    sockaddr_in serverAddress;  // 16 bytes
    int serverFileDescriptor;   // 4 bytes
    int clientFileDescriptor;   // 4 bytes
    std::uint16_t port;         // 2 bytes
};