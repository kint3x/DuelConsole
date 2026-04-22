#pragma once
#include <SDL2/SDL.h>
#include <platform/IPlatform.hpp>

#include <random>
#include <thread>
#include <atomic>

#include <mutex>
#include <CommnTypes.hpp>

struct discoveryDevice
{
    device_id adress;
    GAME_NAME game;
};


class SDLPlatform : public IPlatform {
private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture* texture = nullptr;

    int width;
    int height;
    std::mt19937 rng;   // Mersenne Twister RNG
    
    std::thread advertisingThread;
    std::atomic<bool> advertising{false};
    
    std::thread discoveryThread;
    std::atomic<bool> discovering{false};
    std::map<device_id, GAME_NAME> discoveredDevices;
    std::mutex discoveredMutex;

public:
    bool init(int w, int h) override;
    Input pollInput() override;
    void present(const uint16_t* framebuffer) override;
    uint64_t time() override;
    uint32_t getRandomNumber() override;

    void startAdvertising(GAME_NAME game) override;
    void stopAdvertising() override;
    bool isAdvertising() const override;

    void startDiscovery() override;
    void stopDiscovery() override;
    bool isDiscovering() const override;

    void pollDiscovered(std::vector<device_id>& devicesOut) override;

    
};