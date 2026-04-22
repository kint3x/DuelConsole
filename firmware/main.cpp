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

    //IGame* game = new SlideLama(&engine,&platform,true);
    
    IGame* game = new MainMenu(&engine,&platform);
    game->init();

    bool running = true;
    uint64_t lastTime = platform.time();
    int timeout=2000;
    
    while (running) {

        Input input = platform.pollInput();

        
        if (input.quit) {
            running = false;
        }

        uint64_t currentTime = platform.time();
        uint32_t delta = (uint32_t)(currentTime - lastTime);
        timeout = timeout - delta;
        lastTime = currentTime;
        game->update(&input, delta);
        engine.updateAnimations(delta);
        engine.render();
    }

    return 0;
}