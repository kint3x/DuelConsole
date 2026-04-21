#pragma once

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

struct GameInfo
{
    GAME_NAME name;
    device_id device_ID;
};
