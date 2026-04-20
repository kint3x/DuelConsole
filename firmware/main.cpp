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

    while (running) {
        Input input = platform.pollInput();

        if (input.quit) {
            running = false;
        }

        game->update(&input, platform.time()); 

        engine.render();
    }

    return 0;
}