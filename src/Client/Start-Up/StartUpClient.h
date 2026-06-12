#pragma once
#include <arpa/inet.h>
#include <string>

class Client {
public:
    Client(std::string i, int p) { ip = i, port = p, sock = -1; }
    ~Client();
    void initialiseClientConnection();
    void createClientSocket();
    int getClientSocket() const { return sock; }
    void connectToServer();
private:
    int port;
    int sock;
    std::string ip;
    sockaddr_in serverAddress;
};