#pragma once

#include <cstdint>
#include <ICQEngine/include/ICQEngine.hpp>


class IGame
{
public:
    IGame(ICQEngine* engine) : m_engine(engine){};
    virtual ~IGame() = default;

    // Game lifecycle
    virtual void init() {};
    virtual void update(const Input *input, uint32_t delta) = 0;
    

protected:
    ICQEngine* m_engine;
};