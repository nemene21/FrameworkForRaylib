#ifndef PACKETS_H
#define PACKETS_H
#include <iostream>
#include <cstdint>

struct Packet {
    uint8_t type;
};

struct LogPacket: public Packet {
    char message[256];
};
#endif