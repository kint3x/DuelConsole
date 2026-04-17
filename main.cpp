#include <platform/pc/SDLPlatform.hpp>
#include <ICQEngine/include/ICQEngine.hpp>

#include <games/MainMenu/resources/resources.hpp>

int main() {
    SDLPlatform platform;

    ICQEngine engine(480, 320, &platform);

    bool running = true;

    char text[] = "Test Krasneho textu";

    engine.clean();
    engine.drawSprite(Menu_BCG,0,0);
    engine.drawText(text,FONT_SIZE_24,FONT_SOURCECODEPRO,0x0000,15,15,-10);
    while (running) {
        Input input = platform.pollInput();

        if (input.quit) {
            running = false;
        }


        engine.render();
    }

    return 0;
}