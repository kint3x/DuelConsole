#include <games/MainMenu/MainMenu.hpp>
#include <generated/resources.hpp>
#include <generated/fonts.hpp>

#include <iostream>

MainMenu::~MainMenu() = default;


MainMenu::MainMenu(ICQEngine *engine,IPlatform *plat) : IGame(engine, plat) {
    
    
}

void MainMenu::init() {
    
    //menuStack.push(&mainMenu);
}

void MainMenu::update(const Input *input, uint32_t delta) {
    // Handle input and update menu state

    if(timeoutWaitUntil > 0) {
        timeoutWaitUntil = timeoutWaitUntil-delta;
    }
    else{
        if(input->up || input->down || input->x){
            timeoutWaitUntil = 200; // 200 ms timeout
            if (input->up) {
                moveSelection(-1);
            }
            else if (input->down) {
                moveSelection(1);
            }
            else if(input->x)
            {
                handleMenuClick();
            }
        }  
    }

    m_engine->drawSprite(&Menu_BCG,0,0);
    
     
}

void MainMenu::handleMenuClick(){

}



void MainMenu::drawMenu(Menu *menu) {
    // Draw menu items
    
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
    if(menuStack.top().selectedIndex + direction < 0) return;
    if(menuStack.top().selectedIndex + direction >= menuStack.top().itemCount) return;

}