#pragma once

#include <cstdint>

struct Input {
    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;
    bool x = false;
    bool b = false;
    bool quit = false;
};

typedef uint32_t device_id; 


enum GAME_NAME{
    SLIDEALAMA
};

struct GameMessage{
    uint8_t gameID;
    uint8_t bytes[100];
};

struct LanBroadcastPacket
{
    char magic[4];        // "ICQG"
    uint32_t gameId;
    uint16_t wsPort;      // WebSocket server port
};
