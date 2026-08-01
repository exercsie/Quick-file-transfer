#pragma once

#include <string>
#include "../Start-Up/StartUpServer.h"

struct MenuServer {
    void menuServer(Server &s, std::string& quickPath);
    const bool createFileOption(Server &s);
    void sendWithQuickPath(Server &s, const std::string& quickPath);
};