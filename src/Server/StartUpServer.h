#pragma once

class Server {
public:
    void createServerFileDescriptor();
    int getServerFileDescriptorSocket() const;
    void bindSocket(const int& PORT);
    void listenOnServerFileDescriptor(const int& PORT);
    void acceptConnection();
private:
    int serverFileDescriptor;
};