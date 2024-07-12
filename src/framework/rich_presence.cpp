#include <rich_presence.hpp>

std::string presence_title   = "...";
std::string presence_details = "Developing...";
float presence_time_start    = time(NULL);

void handle_ready(const DiscordUser* connectedUser) {
    printf("DISCORD RPC READY - user: %s\n", connectedUser->username);
}

void RichPresence::init() {
    DiscordEventHandlers handlers;
    memset(&handlers, 0, sizeof(handlers));
    handlers.ready = handle_ready;

    Discord_Initialize("1261279643965591646", &handlers, 1, NULL);
    printf("DISCORD RPC INITIALISED\n");
}

void RichPresence::update() {
    DiscordRichPresence presence;
    memset(&presence, 0, sizeof(presence));
    presence.state = presence_title.c_str();
    presence.details = presence_details.c_str();
    presence.startTimestamp = presence_time_start;

    Discord_UpdatePresence(&presence);
}

void RichPresence::deinit() {
    Discord_Shutdown();
}