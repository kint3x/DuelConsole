#pragma once
#include <SDL2/SDL.h>
#include <platform/IPlatform.hpp>

class SDLPlatform : public IPlatform {
private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture* texture = nullptr;

    int width;
    int height;

public:
    bool init(int w, int h) override;
    Input pollInput() override;
    void present(const uint16_t* framebuffer) override;
    uint64_t time() override;
};