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


/*
*   Draws text with chosen color,size,font
*/
void ICQEngine::drawText(char *text, FONT_SIZE SIZE, FONT_TYPE TYPE, uint16_t color, position_t pos, int8_t spacing)
{
    if (!text) return;

    fontmap_t* font = FONT_REGISTRY[TYPE][SIZE];
    if (!font) return;

    uint16_t glyphSize = font->font_size_px;
    uint16_t cursorX = pos.x;

    for (int i = 0; text[i] != '\0'; i++)
    {
        char c = text[i];

        // handle unsupported chars
        if (c < 33 || c > 126)
        {
            cursorX += glyphSize / 2; // spacing for unknown chars
            continue;
        }

        int index = c - 33;
        int glyphArea = glyphSize * glyphSize;
        int base = index * glyphArea;

        for (uint16_t gy = 0; gy < glyphSize; gy++)
        {
            for (uint16_t gx = 0; gx < glyphSize; gx++)
            {
                uint8_t pixel = font->pixels[base + gy * glyphSize + gx];

                if (pixel)
                {
                    uint16_t px = cursorX + gx;
                    uint16_t py = pos.y + gy;

                    if (px < curr_frame.width && py < curr_frame.height)
                    {
                        curr_frame.pixels[py * curr_frame.width + px] = color;
                    }
                }
            }
        }

        cursorX += glyphSize + spacing; // advance cursor
    }
}

void ICQEngine::drawRect(Rect r, uint8_t color, RECT_FILL_MODE mode, uint8_t thickness)
{
    uint16_t fb_w = curr_frame.width;
    uint16_t fb_h = curr_frame.height;

    // Compute clipped bounds
    uint16_t x0 = r.pos.x;
    uint16_t y0 = r.pos.y;
    uint16_t x1 = (fb_w<r.pos.x + r.w) ? fb_w : r.pos.x + r.w;
    uint16_t y1 = (fb_h<r.pos.y + r.h) ? fb_h : r.pos.y + r.h;

    // Fully outside
    if (x0 >= x1 || y0 >= y1)
        return;

    if (mode == RECT_INSIDE_MODE)
    {
        for (uint16_t y = y0; y < y1; y++)
        {
            uint16_t* row_ptr = curr_frame.pixels + y * fb_w + x0;

            for (uint16_t x = x0; x < x1; x++)
            {
                row_ptr[x - x0] = color;
            }
        }
    }
    else
    {
        if (thickness == 0) return;

        uint16_t max_t = (r.w < r.h ? r.w : r.h) / 2;
        if (thickness > max_t) thickness = max_t;

        // ---- TOP ----
        for (uint16_t y = y0; y < y0 + thickness; y++)
        {
            uint16_t* row = curr_frame.pixels + y * fb_w + x0;
            for (int x = 0; x < r.w; x++)
                row[x] = color;
        }

        // ---- BOTTOM ----
        for (uint16_t y = y1 - thickness; y < y1; y++)
        {
            uint16_t* row = curr_frame.pixels + y * fb_w + x0;
            for (int x = 0; x < r.w; x++)
                row[x] = color;
        }

        // ---- LEFT + RIGHT ----
        for (uint16_t y = y0 + thickness; y < y1 - thickness; y++)
        {
            uint16_t* row = curr_frame.pixels + y * fb_w;

            // left
            for (uint16_t x = x0; x < x0 + thickness; x++)
                row[x] = color;

            // right
            for (uint16_t x = x1 - thickness; x < x1; x++)
                row[x] = color;
        }

    }

}