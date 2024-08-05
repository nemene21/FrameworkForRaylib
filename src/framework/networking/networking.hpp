#ifndef NETWORKING_H
#define NETWORKING_H
#include <iostream>
#include <vector>
#include "packets.hpp"
#include <stdio.h>
#include <stdlib.h>

namespace Networking {
    extern bool is_host;
    extern bool is_client;
    extern int port;

    void init();
    void deinit();

    void host();
    void join(std::string address);
    void disconnect();
    std::string get_local_ip();
    bool active();

    void process();
    void send(Packet* packet, size_t size, bool reliable=false);
}
#endif