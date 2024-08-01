#ifndef PACKETS_H
#define PACKETS_H
#include <iostream>
#include <cstdint>
#include <functional>

enum class PacketType: uint8_t  {
    LOG,
    COUNT,
};

struct Packet {
    PacketType type;
    bool global;
};

struct LogPacket: public Packet {
    char message[256];
};

extern std::function<void(Packet*)> unpackers[(int)PacketType::COUNT];
namespace Packets {
    void init();
}

#endif