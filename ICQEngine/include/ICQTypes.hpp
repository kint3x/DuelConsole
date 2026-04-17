#ifndef ICQ_TYPES_HPP
#define ICQ_TYPES_HPP

#include <cstdint>

#define TRANSPARENT_COLOR 0x0001


struct framebuffer_t
{
    uint16_t width;
    uint16_t height;
    uint16_t *pixels;
};

struct fontmap_t
{
    uint16_t font_size_px;
    uint8_t *pixels;
};


#endif //ICQ_TYPES_HPP