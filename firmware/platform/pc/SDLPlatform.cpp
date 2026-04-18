#include <platform/pc/SDLPlatform.hpp>

bool SDLPlatform::init(int w, int h) {
    width = w;
    height = h;

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("ICQ Console",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        w, h, 0);

    renderer = SDL_CreateRenderer(window, -1, 0);

    texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGB565,   // IMPORTANT (matches framebuffer)
        SDL_TEXTUREACCESS_STREAMING,
        w, h
    );

    return true;
}

void SDLPlatform::present(const uint16_t* fb) {
    SDL_UpdateTexture(texture, nullptr, fb, width * sizeof(uint16_t));

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}

Input SDLPlatform::pollInput() {
    Input in;

    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            in.quit = true;
        }
    }

    const Uint8* keys = SDL_GetKeyboardState(NULL);

    in.up    = keys[SDL_SCANCODE_UP];
    in.down  = keys[SDL_SCANCODE_DOWN];
    in.left  = keys[SDL_SCANCODE_LEFT];
    in.right = keys[SDL_SCANCODE_RIGHT];
    in.a     = keys[SDL_SCANCODE_Z];
    in.b     = keys[SDL_SCANCODE_X];

    return in;
}


float SDLPlatform::time() {
    return SDL_GetTicks() / 1000.0f;
}