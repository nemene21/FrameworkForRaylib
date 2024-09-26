#ifndef PRESENCE_H
#define PRESENCE_H
#include <discord_rpc.h>
#include <raylib.h>
#include <iostream>
#include <stdio.h>
#include <time.h>
#include <cstring>
#include <chrono>

extern std::string presence_title;
extern std::string presence_details;
extern float presence_time_start;

class RichPresence {
public:
    static void init();
    static void update();
    static void deinit();
};

#endif