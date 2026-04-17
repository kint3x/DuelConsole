#include <ICQEngine/include/ICQEngine.hpp>

ICQEngine::ICQEngine(uint16_t width, uint16_t height,IPlatform *p) : platform(p)
{
    curr_frame.width = width;
    curr_frame.height = height;
    curr_frame.pixels = new uint16_t[width * height];
    platform->init(width,height);
}

ICQEngine::~ICQEngine()
{
    delete[] curr_frame.pixels;  // ⚠️ also fixed (see below)
}

void ICQEngine::clean()
{
    for (unsigned i = 0; i < curr_frame.width * curr_frame.height; i++)
    {
        curr_frame.pixels[i] = 0x0000;
    }
}

void ICQEngine::render()
{
    platform->present(curr_frame.pixels);
}