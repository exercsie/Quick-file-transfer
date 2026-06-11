#include "MenuServer.h"
#include "../StartUpServer.h"
#include <iostream>

void menuServer(const int& PORT) {
    Server s(PORT);
    s.initialiseServerConnection();

    std::cout << "  ___        _      _    _____   _     _____                     __           \n";
    std::cout << " / _ \\ _   _(_) ___| | _|  ___(_) | __|_   _| __ __ _ _ __  ___ / _| ___ _ __ \n";
    std::cout << "| | | | | | | |/ __| |/ / |_  | | |/ _ \\| || '__/ _` | '_ \\/ __| |_ / _ \\ '__|\n";
    std::cout << "| |_| | |_| | | (__|   <|  _| | | |  __/| || | | (_| | | | \\__ \\  _|  __/ |   \n";
    std::cout << " \\__\\_\\__,_ |_|\\___|_|\\_\\_|   |_|_|\\___||_||_|  \\__,_|_| |_|___ /_| \\___|_|   \n";
    std::cout << std::endl;

    while(true) {
        std::size_t choice;
        std::cout << "0 - Exit\n";
        std::cout << "1 - Send a file\n";
        std::cout << "2 - Receive a file\n";
        std::cin >> choice;
        if(choice < 0 || choice > 2) {
            std::cerr << "Invalid choice!\n";
            continue;
        }  

        switch(choice) {
            case 0: {
                exit(0);
                break;
            }

            case 1: {

                break;
            }

            case 2: {
                
                break;
            }
        }
    }

}