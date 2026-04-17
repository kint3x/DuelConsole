#ifndef ICQ_ENGINE_HPP
#define ICQ_ENGINE_HPP

#include <ICQEngine/include/ICQTypes.hpp>
#include <platform/IPlatform.hpp>

#define TRANSPARENT_COLOR 0x0001

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
};

#endif //ICQ_ENGINE_HPP