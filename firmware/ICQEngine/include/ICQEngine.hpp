#ifndef ICQ_ENGINE_HPP
#define ICQ_ENGINE_HPP

#include <ICQEngine/include/ICQTypes.hpp>
#include <generated/fonts.hpp>
#include <platform/IPlatform.hpp>



class ICQEngine
{
    private:
        framebuffer_t curr_frame;
        IPlatform *platform;
    public:

    ICQEngine(uint16_t width, uint16_t height,IPlatform *p);

    ~ICQEngine();
    void clean();
    void render();
    void drawSprite(framebuffer_t data, uint16_t x, uint16_t y);
    void drawText(char *text, FONT_SIZE SIZE, FONT_TYPE TYPE, uint16_t color, position_t pos, int8_t spacing);
    void drawRect(Rect r, uint8_t color, RECT_FILL_MODE MODE, uint8_t thickness);
};

#endif //ICQ_ENGINE_HPP