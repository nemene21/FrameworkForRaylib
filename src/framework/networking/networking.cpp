#include "networking.hpp"

namespace Networking {
    bool is_host = false, is_client = false;
    ENetAddress addr;
    ENetHost* server;
    ENetHost* client;
    ENetPeer* client_peer;

    int port = 7777;

    void host() {
        addr.host = ENET_HOST_ANY;
        addr.port = port;

        server = enet_host_create(&addr, 4, 2, 0, 0);
        if (server == NULL) {
            std::cerr << "Error while creating server :(" << std::endl;
            return;
        }
        std::cout << "Server successfully started :D" << std::endl;
        is_host = true;
    }

    void process_host_event(ENetEvent& event) {
        switch (event.type) {
            case ENET_EVENT_TYPE_CONNECT:
                std::cout << event.peer->address.host << std::endl;
                break;
            
            case ENET_EVENT_TYPE_RECEIVE:
                std::cout << "Received: " << std::string((char*)event.packet->data, event.packet->dataLength) << std::endl;
                enet_packet_destroy(event.packet);
                break;
            
            case ENET_EVENT_TYPE_DISCONNECT:
                std::cout << event.peer->data << " disconnected" << std::endl;
                event.peer->data = NULL;
                break;
        }
    }

    void process_host() {
        ENetEvent event;
        while (enet_host_service(server, &event, 1000) > 0) {
            process_host_event(event);
        }
    }

    std::string get_local_ip() {
        system("ipconfig > temp.txt");
        std::string terminal_output;

        FILE* temp = fopen("temp.txt", "r");
        char buff[64];

        while (fgets(buff, 64, temp) != nullptr) {
            terminal_output += buff;
        }

        fclose(temp);
        remove("temp.txt");

        size_t start = terminal_output.find("IPv4 Address");
        start = terminal_output.find(":", start);
        start += 2;

        size_t end = terminal_output.find("\n", start);
        return terminal_output.substr(start, end - start);
    }

    void join(std::string address) {
        client = enet_host_create(NULL, 1, 2, 0, 0);

        if (client == NULL) {
            std::cerr << "Error while making the client :(" << std::endl;
        }

        enet_address_set_host(&addr, address.c_str());
        addr.port = port;

        client_peer = enet_host_connect(client, &addr, 2, 0);
        if (client_peer == NULL) {
            std::cout << "Error while making peer :(" << std::endl;
        }

        ENetEvent event;
        if (enet_host_service(client, &event, 5000) && 
            event.type == ENET_EVENT_TYPE_CONNECT)
        {
            std::cout << "Connected!" << std::endl;
        } else {
            enet_peer_reset(client_peer);
            client_peer = nullptr;
            std::cerr << "Error connecting peer" << std::endl;
            return;
        }

        ENetPacket* packet = enet_packet_create(
            "Hello, server!", 
            strlen("Hello, server!") + 1,
            ENET_PACKET_FLAG_RELIABLE);

        enet_peer_send(client_peer, 0, packet);
        enet_host_flush(client);

        is_client = true;
    }

    void host_disconnect() {
        is_host = false;
        std::vector<ENetPeer*> peers;

        for (int i = 0; i < server->peerCount; i++) {
            ENetPeer* peer = (server->peers + i);
            if (peer->state == ENET_PEER_STATE_CONNECTED) {
                peers.push_back(peer);
            }
        }
        for (auto peer: peers) {
            enet_peer_reset(peer);
        }
        enet_host_destroy(server);
    }

    void client_disconnect() {
        enet_peer_disconnect(client_peer, 0);

        ENetEvent event;
        while (enet_host_service(client, &event, 3000) > 0) {
            if (event.type == ENET_EVENT_TYPE_RECEIVE) {
                enet_packet_destroy(event.packet);
            } else if (event.type == ENET_EVENT_TYPE_DISCONNECT) {
                std::cout << "Disconnected!" << std::endl;
            }
        }

        enet_peer_reset(client_peer);
        enet_host_destroy(client);
        is_client = false;
    }

    void disconnect() {
        std::cout << "Disconnecting..." << std::endl;
        if (is_client) {
            client_disconnect();
        } else if (is_host) {
            host_disconnect();
        }
    }

    void process_client_event(ENetEvent& event) {
        switch (event.type) {
            case ENET_EVENT_TYPE_RECEIVE:
                std::cout << "Received: " << std::string((char*)event.packet->data, event.packet->dataLength) << std::endl;
                enet_packet_destroy(event.packet);
                break;
            
            case ENET_EVENT_TYPE_DISCONNECT:
                std::cout << "Disconnected by the server..." << std::endl;
                is_client = false;
                break;
        }
    }

    void process_client() {
        ENetEvent event;

        while (enet_host_service(client, &event, 1000) > 0) {
            process_client_event(event);
        }
    }

    void send_by_client() {
        
    }

    void send_by_server() {

    }

    void send() {
        if (is_client) {
            send_by_client();
        } else {
            send_by_server();
        }
    }

    void process() {
        if (is_client) {
            process_client();
        } else {
            process_host();
        }
    }

    bool active() {
        return is_client || is_host;
    }
}