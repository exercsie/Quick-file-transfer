#pragma once

constexpr std::size_t BUFFERSIZE = 262144;

enum MessageType {
    TYPE_EXIT = 0, 
    TYPE_SEND = 1,
    TYPE_RECEIVE = 2
};