#include <platform/pc/SDLPlatform.hpp>
#include <ICQEngine/include/ICQEngine.hpp>

#include <generated/resources.hpp>

int main() {
    SDLPlatform platform;

    ICQEngine engine(480, 320, &platform);

    bool running = true;

    char text[] = "Test Krasneho textu";

    engine.clean();
    engine.drawSprite(Menu_BCG,0,0);
    engine.drawText(text,FONT_SIZE_24,FONT_SOURCECODEPRO,0x0000,{200,15},-10);
    engine.drawRect({{30,30},50,50},0xFFFF,RECT_OUTLINE_MODE,4);
    while (running) {
        Input input = platform.pollInput();

        if (input.quit) {
            running = false;
        }


        engine.render();
    }

    return 0;
}