#include "Helper.h"

#include <iostream>
#include <sys/socket.h>
#include <print>
#include <unistd.h>


ssize_t Distribute::sendAll(int sock, const char* data, std::size_t len) {
    ssize_t size = len;
    
    // send size
    ssize_t sent{};
    while(sent < sizeof(size)) {
        ssize_t bytesSend = send(sock, reinterpret_cast<const char*>(&size) + sent, sizeof(len) - sent, 0);
        if(bytesSend <= 0) {
            return -1;
        }

        sent += bytesSend;
    }
    
    std::println("Size is: {}", len);
        
    // send data
    ssize_t totalSent{};
    while(totalSent < static_cast<ssize_t>(len)) {
        ssize_t bytesSend = send(sock, data + totalSent, len - totalSent, 0);
        if(bytesSend <= 0) {
            return -1;
        }
        totalSent += bytesSend;
        if(totalSent == len) {
            std::println("Bytes sent: {}", totalSent);
            std::cout << "exited sendAll\n";
            return totalSent;
        }
    }

    return 0;
}

ssize_t Distribute::recvAll(int sock, char* data) {
    std::size_t len{};

    // receive size
    ssize_t received{};
    while(received < static_cast<ssize_t>(sizeof(len))) {
        ssize_t bytesRec = recv(sock, reinterpret_cast<char*>(&len) + received, sizeof(len) - received, 0);
        if(bytesRec <= 0) {
            return -1;
        }
        received += bytesRec;  
    }
    
    std::println("Size is: {}", len);
      
    
    // receive data
    ssize_t total{};
    while(total < len) {
        ssize_t bytesRec = recv(sock, data + total, len - total, 0);
        if(bytesRec <= 0) {
            return -1;
        }

        total += bytesRec;
        std::println("total {}", total);
        if(total == len) {
            std::println("Bytes recevied: {}", total);
            std::cout << "exited recvall\n";
            return total;
        }
    }

    return 0;
}
