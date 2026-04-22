#pragma once
#include <cstdint>
#include <CommnTypes.hpp>
#include <map>
#include <vector>




class IPlatform {
public:
    virtual ~IPlatform() = default;

    virtual bool init(int width, int height) = 0;

    virtual Input pollInput() = 0;

    virtual void present(const uint16_t* framebuffer) = 0;

    virtual uint32_t getRandomNumber()=0;

    virtual uint64_t time() = 0;

    // Network methods:

    /*
    // Return true if there is new message for game and fills it to gameMessage
    virtual bool pollForEventMessage(GameMessage& gameMessage) = 0; 
    virtual bool pollForStateMessage(GameMessage& gameMessage) = 0; 

    // Sends message to currently connected device
    virtual bool sendMessage(device_id target,GameMessage& gameMessage) = 0;
    */

    // non blocking advertising game creation and returns id of device that will be handle for sending comm
    virtual void startAdvertising(GAME_NAME game) = 0;
    virtual void stopAdvertising() = 0;
    virtual bool isAdvertising() const = 0;
    
    //starts discovery process
    virtual void startDiscovery() = 0;
    virtual void stopDiscovery() = 0;
    virtual bool isDiscovering() const = 0;
    
    //Polls discovered items in form of map of devices_id, device_id is abstraction id of device

    
    virtual void pollDiscovered(std::vector<device_id>& devicesOut) = 0;


};