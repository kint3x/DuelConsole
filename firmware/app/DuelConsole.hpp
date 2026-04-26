#pragma once

#include <ICQEngine/include/ICQEngine.hpp>

struct PlatformAPI
{

};  


class DuelConsole
{
private:
    PlatformAPI *device;
    ICQEngine engine;

public:
    DuelConsole(PlatformAPI *deviceAPI);
    ~DuelConsole();

    void run();
};

