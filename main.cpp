#include <platform/pc/SDLPlatform.hpp>
#include <ICQEngine/include/ICQEngine.hpp>

int main() {
    SDLPlatform platform;

    ICQEngine engine(480, 320, &platform);

    bool running = true;

    while (running) {
        Input input = platform.pollInput();

        if (input.quit) {
            running = false;
        }

        engine.clean();
        engine.render();
    }

    return 0;
}