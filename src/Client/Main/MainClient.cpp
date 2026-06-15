#include <iostream>

#include "MenuClient.h"
#include "../Start-Up/StartUpClient.h"


int main(int argc, char* argv[]) {
    if(argc < 3) {
        throw std::runtime_error("Please include IP and PORT \"./Client [IP] [PORT]\"" );
    }

    std::string IP = argv[1];
    int PORT = std::atoi(argv[2]);

    Client client(IP, PORT);

    menuClient(client);   
}