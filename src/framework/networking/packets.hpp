#ifndef PACKETS_H
#define PACKETS_H
#include <iostream>
#include <cstdint>
#include <functional>

enum class EntityType: uint16_t {
    PLAYER,
    COUNT,
};

enum class PacketType: uint8_t  {
    LOG,
    ENTITY_SYNC,
    COUNT,
};

struct Packet {
    PacketType type;
    bool global;
};

struct LogPacket: public Packet {
    char message[256];
};

struct EntitySyncPacket: public Packet {
    EntityType entity_type;
    uint32_t id;
    bool owned;
};

extern std::function<void(Packet*)> unpackers[(int)PacketType::COUNT];
namespace Packets {
    void init();
}

#endif