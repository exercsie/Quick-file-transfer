#include <iostream>
#include "MenuServer.h"

int main(int argc, char* argv[]) {
    if(argc < 2) {
        throw std::runtime_error("Please include PORT \"./Server [PORT]\"");
    }

    int PORT = std::atoi(argv[1]);

    menuServer(PORT);
}