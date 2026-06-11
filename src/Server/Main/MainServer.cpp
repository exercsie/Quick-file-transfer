#include "MenuServer.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    int PORT = std::atoi(argv[1]);
    std::string quickPath;
    // if one argument is entered quickpath is < 1

    switch(argc) {
        case 2: {
            menuServer(PORT, quickPath);
            break;
        }

        case 3: {
            quickPath = argv[2];
            menuServer(PORT, quickPath);
            break;
        }

        default: {
            throw std::runtime_error("Please include PORT \"./Server [PORT]\"");
        }
    }


    
}