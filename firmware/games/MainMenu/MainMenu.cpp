#include <games/MainMenu/MainMenu.hpp>
#include <generated/resources.hpp>
#include <generated/fonts.hpp>

#include <iostream>

MainMenu::~MainMenu() = default;


MainMenu::MainMenu(ICQEngine *engine) : IGame(engine) {
    
    
}

void MainMenu::init() {
    m_engine->drawSprite(&Menu_BCG,0,0);
        
    menuStack.push(&mainMenu);

    drawMenu(menuStack.top());
}



void MainMenu::update(const Input *input, uint64_t globtime) {
    // Handle input and update menu state
    (void) input;

    if(globtime < timeoutWaitUntil) {
        std::cout << "Input timeout, ignoring input. Time left: " << (timeoutWaitUntil - globtime) << " ms" << std::endl;
        return; // Still in timeout, ignore input
    }
    if(input->up || input->down){
        timeoutWaitUntil = globtime + 200; // 200 ms timeout
    }
    //handle inputs
    if (input->up) {
        moveSelection(-1);
    }
    else if (input->down) {
        moveSelection(1);
    }
    
}



void MainMenu::drawMenu(Menu *menu) {
    // Draw menu items
    position_t position = menu->start_position;
    for (int i = 0; i < menu->itemCount; ++i) {
        drawMenuItem(&(menu->items[i]), i == menu->selectedIndex, &position);
        position.y += font_size_to_px[menu->fontSize]; // Move down for the next item
    }
}

void MainMenu::drawMenuItem(MenuItem *item, bool selected, position_t *position) {
    uint16_t color = 0x0000;
    FONT_TYPE type;
    if(selected) {
        type = FONT_SOURCECODEPROBOLD;
    }
    else{
        type = FONT_SOURCECODEPRO;
    }
    m_engine->drawText(item->text.c_str(), FONT_SIZE_32, type, color, *position, -10);
}

void MainMenu::moveSelection(int direction) {
    // Move selection up or down
    std::cout << "Moving selection: " << direction << std::endl;
    if(menuStack.top()->selectedIndex + direction < 0) return;
    if(menuStack.top()->selectedIndex + direction >= menuStack.top()->itemCount) return;
    

    Rect r = {
        .pos = {menuStack.top()->start_position.x, 
            menuStack.top()->start_position.y + menuStack.top()->selectedIndex * font_size_to_px[menuStack.top()->fontSize]},
        .w = 250, // Assuming full width for highlight
        .h = font_size_to_px[menuStack.top()->fontSize]
    };

    m_engine->drawRect(r,0xd6b9,RECT_INSIDE_MODE,0);

    drawMenuItem(&menuStack.top()->items[menuStack.top()->selectedIndex], false, &r.pos);

    menuStack.top()->selectedIndex += direction;
    r.pos.y += direction * font_size_to_px[menuStack.top()->fontSize];
    drawMenuItem(&menuStack.top()->items[menuStack.top()->selectedIndex], true, &r.pos);
    

}