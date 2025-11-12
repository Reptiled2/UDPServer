#pragma once

#include "buffer.h"

enum PacketTypes {
    CONNECT = 1,
    DISCONNECT = 2,
    MESSAGE = 3,
    KEEPALIVE = 4,
    ALREADY_CONNECTED = 5
};


struct Packet {
    std::vector<int> peerId;
    PacketTypes type;
    BufferVector buffer;
};
