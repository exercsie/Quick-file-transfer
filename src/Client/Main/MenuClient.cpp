#include "MenuClient.h"
#include "../StartUpClient.h"
#include <iostream>
#include <string>

void menuClient(std::string& IP, const int& PORT) {
    Client c(IP, PORT);
    c.initialiseClientConnection();

    std::cout << "  ___        _      _    _____   _     _____                     __           \n";
    std::cout << " / _ \\ _   _(_) ___| | _|  ___(_) | __|_   _| __ __ _ _ __  ___ / _| ___ _ __ \n";
    std::cout << "| | | | | | | |/ __| |/ / |_  | | |/ _ \\| || '__/ _` | '_ \\/ __| |_ / _ \\ '__|\n";
    std::cout << "| |_| | |_| | | (__|   <|  _| | | |  __/| || | | (_| | | | \\__ \\  _|  __/ |   \n";
    std::cout << " \\__\\_\\__,_ |_|\\___|_|\\_\\_|   |_|_|\\___||_||_|  \\__,_|_| |_|___ /_| \\___|_|   \n";
    std::cout << std::endl;

    while(true) {
        std::cin;
    }
}