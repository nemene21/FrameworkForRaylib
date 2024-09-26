#include "packets.hpp"

std::function<void(Packet*)> unpackers[(int)PacketType::COUNT] = {};

void Packets::init() {
    unpackers[(int)PacketType::LOG] = [](Packet* packet) {
        auto log_packet = reinterpret_cast<LogPacket*>(packet);
        std::cout << log_packet->message << std::endl;
    };
}
 