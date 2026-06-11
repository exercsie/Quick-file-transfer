#pragma once
#include <arpa/inet.h>

class Server {
public:
    Server(int p) { port = p, serverFileDescriptor = -1, clientFileDescriptor = -1; }
    ~Server();
    void initialiseServerConnection();
    void createServerFileDescriptor();
    int getServerFileDescriptorSocket() const;
    void bindSocket();
    void listenOnServerFileDescriptor();
    void acceptConnection();
    int getClientFileDescriptor();
private:
    int port;
    int serverFileDescriptor;
    int clientFileDescriptor;
    sockaddr_in serverAddress;
};