#pragma once

#include <cstdint>
#include <platform/IPlatform.hpp>
#include <ICQEngine/include/ICQEngine.hpp>

class IGame
{
public:
    IGame(ICQEngine* engine) : m_engine(engine){};
    virtual ~IGame() = default;

    // Game lifecycle
    virtual void init() {};
    virtual void update(const Input *input, uint64_t globtime) = 0;

    // State management
    virtual bool isRunning() const = 0;
    virtual void setRunning(bool running) = 0;

protected:
    ICQEngine* m_engine;
    bool IsRunning;
};