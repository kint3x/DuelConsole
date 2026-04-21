#pragma once
#include <cstdint>
#include <CommnTypes.hpp>

class IPlatform {
public:
    virtual ~IPlatform() = default;

    virtual bool init(int width, int height) = 0;

    virtual Input pollInput() = 0;

    virtual void present(const uint16_t* framebuffer) = 0;

    virtual uint32_t getRandomNumber()=0;

    virtual uint64_t time() = 0;

    // Network methods:
    
    // non blocking advertising game creation and returns id of device that will be handle for sending comm
    virtual void startAdvertising(const GameInfo& game) = 0;
    virtual void stopAdvertising() = 0;
    virtual bool isAdvertising() const = 0;


};