#ifndef PACKET_H
#define PACKET_H
#include <array>
#include "Buffer.h"

typedef enum PacketTypes {
    CONNECT = 1,
    DISCONNECT = 2,
    MESSAGE = 3,
    KEEPALIVE = 4,
    ALREADY_CONNECTED = 5
};


typedef struct Packet {
    std::vector<int> peerId;
    PacketTypes type;
    BufferVector buffer;
};

#endif