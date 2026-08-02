#pragma once

#include <arpa/inet.h>
#include <string>
#include <cstdint>

class Client {
public:
    // constructor
    explicit Client(const std::string& i, std::uint16_t p) noexcept : ip(i), serverAddress{}, sock(-1), port(p) {}

    // destructor
    ~Client() noexcept;

    // wrapper
    void initialiseClientConnection() noexcept;

    // core client connectors
    void createClientSocket() noexcept;
    void connectToServer() noexcept;

    // getter
    [[nodiscard]] int getClientSocket() const noexcept{ return sock; }
private:
    std::string ip;             // 32 bytes
    sockaddr_in serverAddress;  // 16 bytes
    int sock;                   // 4 bytes
    std::uint16_t port;         // 2 bytes
};