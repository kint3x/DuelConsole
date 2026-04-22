#pragma once
#include <SDL2/SDL.h>
#include <platform/IPlatform.hpp>

#include <random>
#include <CommnTypes.hpp>

struct LanBroadcastPacket
{
    char magic[4];        // "ICQG"
    uint32_t gameId;
    uint16_t wsPort;      // WebSocket server port
};


class SDLPlatform : public IPlatform {
private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture* texture = nullptr;

    int width;
    int height;
    std::mt19937 rng;   // Mersenne Twister RNG

    bool advertising;
public:
    bool init(int w, int h) override;
    Input pollInput() override;
    void present(const uint16_t* framebuffer) override;
    uint64_t time() override;
    uint32_t getRandomNumber() override;

    void startAdvertising(GAME_NAME game) override;
    void stopAdvertising()override;
    bool isAdvertising() const override;

    
};