#ifndef NETWORKING_H
#define NETWORKING_H
#include <iostream>
#include <vector>
#include "packets.hpp"
#include <stdio.h>

namespace Networking {
    extern bool is_host, is_client;
    extern int port;

    void host();
    void join(std::string address);
    void disconnect();
    std::string get_local_ip();
    bool active();

    void process();
    void send(Packet* packet, size_t size, bool reliable=false);
}
#endif