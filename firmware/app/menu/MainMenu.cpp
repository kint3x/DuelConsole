#include <app/menu/MainMenu.hpp>
#include <generated/resources.hpp>

MainMenu::MainMenu(ICQEngine* engine, GameEntity *e):engine(engine),game(e)
{
}

MainMenu::~MainMenu()
{
}

MenuRetCode MainMenu::update(Input *input, uint32_t delta){



    drawState();

    return MENU_OK;
}

void MainMenu::drawState(){
    // Draw background 
    engine->drawSprite(&Menu_BCG,0,0);
}
