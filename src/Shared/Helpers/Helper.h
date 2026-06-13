#pragma once

#include <iostream>

struct Distribute {
    ssize_t sendAll(int sock, const char* data, std::size_t len);
    ssize_t recvAll(int sock, char* data);
};
