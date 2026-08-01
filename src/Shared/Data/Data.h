#pragma once

#include <iostream>

constexpr std::size_t BUFFERSIZE = 524288;

enum class MessageType : std::uint8_t {
    TYPE_EXIT = 0, 
    TYPE_SEND = 1,
    TYPE_RECEIVE = 2
};

// overload >> operator so that std::cin operations are instantly casted to type MessageType
inline std::istream& operator>>(std::istream& x, MessageType& type) {
    int value;
    if(x >> value) {
        type = static_cast<MessageType>(value);
    }

    return x;
}