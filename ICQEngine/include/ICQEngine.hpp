#ifndef ICQ_ENGINE_HPP
#define ICQ_ENGINE_HPP

#include <ICQEngine/include/ICQTypes.hpp>
#include <ICQEngine/resources/fonts/fonts.hpp>
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
    void drawText(char *text, FONT_SIZE SIZE, FONT_TYPE TYPE, uint16_t color, uint16_t x, uint16_t y, int8_t spacing);
};

#endif //ICQ_ENGINE_HPP