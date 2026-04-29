#pragma once
#include <ICQEngine/include/ICQEngine.hpp>
#include <app/CommonTypes.hpp>
struct PlatformAPI
{
    void (*pollInput)(Input*);
};  


class DuelConsole
{
private:
    PlatformAPI *device;
    ICQEngine engine;

public:
    DuelConsole(PlatformAPI *deviceAPI);
    ~DuelConsole();
    uint32_t getRand();

    void run();
};

