#ifndef ICQ_TYPES_HPP
#define ICQ_TYPES_HPP

#include <cstdint>

#define TRANSPARENT_COLOR 0x0001

enum RECT_FILL_MODE{
    RECT_INSIDE_MODE,
    RECT_OUTLINE_MODE
};

struct position_t
{
    uint16_t x;
    uint16_t y;
};

struct Rect {
    position_t pos;
    uint16_t w, h;
};

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