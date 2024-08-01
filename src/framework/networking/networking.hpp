#ifndef NETWORKING_H
#define NETWORKING_H
#include <iostream>
#include <vector>
#include <stdio.h>
#include "packets.hpp"

namespace Networking {
    #include <enet/enet.h>

    extern bool is_host, is_client;
    extern int port;

    extern ENetAddress addr;
    extern ENetHost* server;

    void host();
    void join(std::string address);
    void disconnect();
    std::string get_local_ip();
    bool active();

    void process();
    void send(Packet* packet, size_t size, bool reliable=false);
}
#endif