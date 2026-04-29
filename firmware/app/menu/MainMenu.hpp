#pragma once
#include <ICQEngine/include/ICQTypes.hpp>
#include <ICQEngine/include/ICQEngine.hpp>
#include <app/CommonTypes.hpp>

class MainMenu
{
private:
    ICQEngine* engine;
public:
    MainMenu(ICQEngine* engine);
    ~MainMenu();

    void update(Input *input, uint32_t delta);
    void poll
};

