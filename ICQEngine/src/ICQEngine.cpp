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

/*
*   Draws input sprite on origin of left top origin x,y
*/
void ICQEngine::drawSprite(framebuffer_t data, uint16_t x, uint16_t y)
{
    for (unsigned sy = 0; sy < data.height; sy++)
    {
        unsigned dy = y + sy;

        if (dy >= curr_frame.height)
            break;

        unsigned row_offset = dy * curr_frame.width;

        for (unsigned sx = 0; sx < data.width; sx++)
        {
            unsigned dx = x + sx;

            if (dx >= curr_frame.width)
                break;

            uint16_t pixel = data.pixels[sy * data.width + sx];

            if (pixel != TRANSPARENT_COLOR)
            {
                curr_frame.pixels[row_offset + dx] = pixel;
            }
        }
    }
}