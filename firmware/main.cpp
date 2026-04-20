#include <platform/pc/SDLPlatform.hpp>
#include <ICQEngine/include/ICQEngine.hpp>
#include <games/MainMenu/MainMenu.hpp>
#include <games/SlideLama/SlideLama.hpp>

#include <generated/resources.hpp>
#include <iostream>
#include <chrono>

int main() {
    SDLPlatform platform;

    ICQEngine engine(480, 320, &platform);

    Settings settings;

    IGame* game = new SlideLama(&engine);

    game->init();

    bool running = true;
    uint64_t lastTime = platform.time();
    while (running) {
        Input input = platform.pollInput();

        if (input.quit) {
            running = false;
        }

        uint64_t currentTime = platform.time();
        uint32_t delta = (uint32_t)(currentTime - lastTime);
        lastTime = currentTime;
        game->update(&input, delta);
        engine.updateAnimations(delta);

        engine.render();
    }

    return 0;
}