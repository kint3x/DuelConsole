#pragma once

#include <cstdint>
#include <platform/IPlatform.hpp>
#include <ICQEngine/include/ICQEngine.hpp>


class IGame
{
public:
    IGame(ICQEngine* engine, IPlatform* plat) : m_engine(engine), platform(plat){};
    virtual ~IGame() = default;

    // Game lifecycle
    virtual void init() {};
    virtual void update(const Input *input, uint32_t delta) = 0;
    
    /* State management
    virtual bool isRunning() const = 0;
    virtual void setRunning(bool running) = 0;*/

protected:
    ICQEngine* m_engine;
    IPlatform* platform;
};