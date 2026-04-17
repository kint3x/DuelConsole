#ifndef ICQ_ENGINE_HPP
#define ICQ_ENGINE_HPP

#include <ICQEngine/include/ICQTypes.hpp>
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
};

#endif //ICQ_ENGINE_HPP