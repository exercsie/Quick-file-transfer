#include <iostream>
#include "MenuClient.h"

int main(int argc, char* argv[]) {
    if(argc < 3) {
        throw std::runtime_error("Please include IP and PORT \"./Server [IP] [PORT]\"" );
    }

    std::string IP = argv[1];
    int PORT = std::atoi(argv[2]);

    menuClient(IP, PORT);   
}