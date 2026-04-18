#pragma once
#include <cstdint>

struct Input {
    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;
    bool a = false;
    bool b = false;
    bool quit = false;
};

class IPlatform {
public:
    virtual ~IPlatform() = default;

    virtual bool init(int width, int height) = 0;

    virtual Input pollInput() = 0;

    virtual void present(const uint16_t* framebuffer) = 0;

    virtual float time() = 0;
};