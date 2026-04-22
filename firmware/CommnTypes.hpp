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

typedef int32_t device_id; 


enum GAME_NAME{
    SLIDEALAMA
};

enum GameMessageType{
    STATE, //state can be overwritten by other state
    EVENT //events must be stored in queue
};
struct GameMessage{
    uint8_t type;
    uint8_t gameID;
    uint8_t bytes[100];
};