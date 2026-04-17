#ifndef ICQ_TYPES_HPP
#define ICQ_TYPES_HPP

#include <cstdint>

struct framebuffer_t
{
    uint16_t width;
    uint16_t height;
    uint16_t *pixels;
};

#endif //ICQ_TYPES_HPP